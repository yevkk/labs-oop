#include <iostream>
#include <vector>

template<typename T, typename M>
class Graph {
public:
    virtual void print() = 0; //TODO: (?) void print(Graph<T, M>);
    virtual bool check_nodes_adjacency(int index1, int index2) = 0;

    virtual void add_node(T data) = 0;

    virtual void add_edge(int index1, int index2, M data) = 0;
};

template<typename T>
class GraphEdge {
public:
    T data;

    explicit GraphEdge(const T Data) {
        data = Data;
    }
};

template<typename T>
class GraphNode {
public:
    T data;

    GraphNode() = default;

    explicit GraphNode(const T Data) {
        data = Data;
    }
};

template<typename T, typename M>
class GraphNodeAdjStr : public GraphNode<T> {
public:
    std::vector<std::pair<int, GraphEdge<M> *>> adjacent_nodes;

    explicit GraphNodeAdjStr(int Data) {
        this->data = Data;
    }
};

template<typename T, typename M>
class GraphAdjStr : public Graph<T, M>{
public:
    std::vector<GraphNodeAdjStr<T, M> *> nodes;
    bool directed;

    explicit GraphAdjStr(bool Directed = false) {
        directed = Directed;
    }

    ~GraphAdjStr() {
        for (auto &node:nodes) delete node;
    }

    void print() override {
        std::cout << "Graph:" << std::endl;
        std::cout << "directed: " << std::boolalpha << directed << std::endl;

        std::cout << "   ";
        for (int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < 3 - std::to_string(i).size(); j++)
                std::cout << " ";
            std::cout << i;
        }
        std::cout << std::endl;

        std::cout << "   ";
        for (int i = 0; i < nodes.size(); i++) {
            std::cout << "---";
        }
        std::cout << std::endl;

        for (int i = 0; i < nodes.size(); i++) {

            for (int j = 0; j < 2 - std::to_string(i).size(); j++)
                std::cout << " ";
            std::cout << i << "|";

            for (int k = 0; k < nodes.size(); k++) {
                for (int j = 0; j < 2; j++)
                    std::cout << " ";
                std::cout << (int) check_nodes_adjacency(i, k);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

    }

    bool check_nodes_adjacency(int index1, int index2) override {
        for (auto &e:nodes[index1]->adjacent_nodes) {
            if (e.first == index2) {
                return true;
            }
        }
        return false;
    }

    void add_node(T data) override {
        nodes.push_back(new GraphNodeAdjStr<T, M>(data));
    }

    void add_edge(int index1, int index2, M data) override {
        if (!(index1 < nodes.size() && index2 < nodes.size())) return;

        auto edge = new GraphEdge<M>(data);
        nodes[index1]->adjacent_nodes.emplace_back(index2, edge);
        if (!directed)
            nodes[index2]->adjacent_nodes.emplace_back(index1, edge);
    }
};

template<typename T, typename M>
class GraphMtrx : public Graph<T, M> {
public:
    std::vector<GraphNode<T> *> nodes;
    std::vector<std::vector<std::pair<bool, GraphEdge<M> *>>> edges;
    bool directed;

    explicit GraphMtrx(bool Directed = false) {
        directed = Directed;
    }

    ~GraphMtrx() {
        for (auto &node:nodes) delete node;
        for (auto &edge:edges)
            for (int i = 0; i < edge.size(); i++)
                delete edge[i].second;
    }

    void print() override {
        std::cout << "Graph:" << std::endl;
        std::cout << "directed: " << std::boolalpha << directed << std::endl;

        std::cout << "   ";
        for (int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < 3 - std::to_string(i + 1).size(); j++)
                std::cout << " ";
            std::cout << i;
        }
        std::cout << std::endl;

        std::cout << "   ";
        for (int i = 0; i < nodes.size(); i++) {
            std::cout << "---";
        }
        std::cout << std::endl;

        for (int i = 0; i < nodes.size(); i++) {

            for (int j = 0; j < 2 - std::to_string(i).size(); j++)
                std::cout << " ";
            std::cout << i << "|";

            for (int k = 0; k < nodes.size(); k++) {
                for (int j = 0; j < 2; j++)
                    std::cout << " ";
                std::cout << (int) check_nodes_adjacency(i, k);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

    }

    bool check_nodes_adjacency(int index1, int index2) override {
        if(index1 < nodes.size() && index2 < nodes.size()){
         return edges[index1][index2].first;
        } else return false;
    }

    void add_node(T data) override {
        static const std::pair<bool, GraphEdge<M> *> tmp = {false, nullptr};
        static const std::vector<std::pair<bool, GraphEdge<M> *>> tmp_vec = {tmp};
        nodes.push_back(new GraphNodeAdjStr<T, M>(data));
        if (edges.empty()) {
            edges.emplace_back(tmp_vec);
        } else {
            edges.emplace_back();
            for (int i = 0; i < edges.size(); i++) {
                edges[i].emplace_back(tmp);
                edges[edges.size() - 1].emplace_back(tmp);
            }
        }
    }

    void add_edge(int index1, int index2, M data) override {
        if (!(index1 < nodes.size() && index2 < nodes.size())) return;

        auto edge = new GraphEdge<M>(data);
        edges[index1][index2].first = true;
        edges[index1][index2].second = edge;
        if (!directed){
            edges[index2][index1].first = true;
            edges[index2][index1].second = edge;
        }
    }
};

void test_int(Graph<int, double> *graph) {
    for (int i = 0; i < 10; i++) {
        graph->add_node(i);
    }
    graph->add_edge(1, 2, 1.2);
    graph->add_edge(2, 5, 4.6);
    graph->add_edge(4, 8, 0);
    graph->add_edge(4, 5, 7.9);
    graph->add_edge(1, 9, 1.1);
    graph->add_edge(2, 2, -9);
    graph->print();
}


int main() {
    auto G1 = new GraphAdjStr<int, double>();
    test_int(G1);
    auto G2 = new GraphMtrx<int, double>();
    test_int(G2);

    return 0;
}