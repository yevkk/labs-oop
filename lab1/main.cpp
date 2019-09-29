#include <iostream>
#include <vector>

template<typename T, typename M>
class Graph {
public:
    virtual void print() = 0;

    virtual bool nodes_adjacency(int index1, int index2) = 0;

    virtual bool adjacent(int index) = 0;

    virtual void add_node(T data) = 0;

    virtual void add_edge(int index1, int index2, M data) = 0;

    virtual int find_node(T key) = 0; //returns index of first element equal to key or -1 if there is no such element;
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
class GraphAdjStr : public Graph<T, M> {
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
        std::cout << "========== [ Graph | " << "directed: " << std::boolalpha << directed << " ] ==========" << std::endl;

        std::cout << "Nodes:" << std::endl;
        for (unsigned int i = 0; i < nodes.size(); i++) {
            std::cout << i << ')' << std::endl;
            std::cout << nodes[i]->data << std::endl;
            std::cout << std::endl;
        }

        std::cout << "Edges:" << std::endl;
        for (unsigned int i = 0; i < nodes.size(); i++) {
            for (auto &e:nodes[i]->adjacent_nodes) {
                if ((!directed && e.first <= i) || directed) {
                    std::cout << i << " - " << e.first << ')' << std::endl;
                    std::cout << e.second->data << std::endl;
                    std::cout << std::endl;
                }
            }
        }

        std::cout << "Adjacency Matrix:" << std::endl;
        std::cout << "   ";
        for (unsigned int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < 3 - std::to_string(i).size(); j++)
                std::cout << " ";
            std::cout << i;
        }
        std::cout << std::endl;

        std::cout << "   ";
        for (unsigned int i = 0; i < nodes.size(); i++) {
            std::cout << "---";
        }
        std::cout << std::endl;

        for (unsigned int i = 0; i < nodes.size(); i++) {

            for (int j = 0; j < 2 - std::to_string(i).size(); j++)
                std::cout << " ";
            std::cout << i << "|";

            for (unsigned int k = 0; k < nodes.size(); k++) {
                for (int j = 0; j < 2; j++)
                    std::cout << " ";
                std::cout << (int) nodes_adjacency(i, k);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

    }

    bool nodes_adjacency(int index1, int index2) override {
        for (auto &e:nodes[index1]->adjacent_nodes) {
            if (e.first == index2) {
                return true;
            }
        }
        return false;
    }

    bool adjacent(int index) override {
        static std::vector<bool> visited;
        static bool start = true;
        if (start) {
            start = false;
            for (auto &e:nodes)
                visited.push_back(false);

            adjacent(0);

            for (unsigned int i = 1; i < nodes.size(); i++) {
                if (!visited[i]) {
                    start = true;
                    return false;
                }
            }
            start = true;
        } else {
            visited[index] = true;
            for (auto &e:nodes[index]->adjacent_nodes)
                if (!visited[e.first]) adjacent(e.first);
        }
        return true;
    }

    void add_node(T data) override {
        nodes.push_back(new GraphNodeAdjStr<T, M>(data));
    }

    void add_edge(int index1, int index2, M data) override {
        if (!(index1 < nodes.size() && index2 < nodes.size())) return;

        auto edge = new GraphEdge<M>(data);
        nodes[index1]->adjacent_nodes.emplace_back(index2, edge);
        if (!directed && index1 != index2)
            nodes[index2]->adjacent_nodes.emplace_back(index1, edge);
    }

    int find_node(T key) override {
        for (unsigned int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->data == key) return (int) i;
        }
        return -1;
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
        std::cout << "========== [ Graph | " << "directed: " << std::boolalpha << directed << " ] ==========" << std::endl;

        std::cout << "Nodes:" << std::endl;
        for (unsigned int i = 0; i < nodes.size(); i++) {
            std::cout << i << ')' << std::endl;
            std::cout << nodes[i]->data << std::endl;
            std::cout << std::endl;
        }

        int num;
        std::cout << "Edges:" << std::endl;
        for (unsigned int i = 0; i < nodes.size(); i++) {
            num = directed ? 0 : i;
            for (unsigned int j = num; j < nodes.size(); j++) {
                if (edges[i][j].first) {
                    std::cout << i << " - " << j << ')' << std::endl;
                    std::cout << edges[i][j].second->data << std::endl;
                    std::cout << std::endl;
                }
            }

        }

        std::cout << "Adjacency Matrix:" << std::endl;
        std::cout << "   ";
        for (unsigned int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < 3 - std::to_string(i).size(); j++)
                std::cout << " ";
            std::cout << i;
        }
        std::cout << std::endl;

        std::cout << "   ";
        for (unsigned int i = 0; i < nodes.size(); i++) {
            std::cout << "---";
        }
        std::cout << std::endl;

        for (unsigned int i = 0; i < nodes.size(); i++) {

            for (int j = 0; j < 2 - std::to_string(i).size(); j++)
                std::cout << " ";
            std::cout << i << "|";

            for (unsigned int k = 0; k < nodes.size(); k++) {
                for (int j = 0; j < 2; j++)
                    std::cout << " ";
                std::cout << (int) nodes_adjacency(i, k);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

    }

    bool nodes_adjacency(int index1, int index2) override {
        if (index1 < nodes.size() && index2 < nodes.size()) {
            return edges[index1][index2].first;
        } else return false;
    }

    bool adjacent(int index) override {
        static std::vector<bool> visited;
        static bool start = true;
        if (start) {
            start = false;
            for (auto &e:nodes)
                visited.push_back(false);

            adjacent(0);

            for (unsigned int i = 1; i < nodes.size(); i++) {
                if (!visited[i]) {
                    start = true;
                    return false;
                }
            }
            start = true;
        } else {
            visited[index] = true;
            for (unsigned int j = 0; j < nodes.size(); j++)
                if (!visited[j] && edges[index][j].first) adjacent(j);
        }
        return true;
    }

    void add_node(T data) override {
        static const std::pair<bool, GraphEdge<M> *> tmp = {false, nullptr};
        static const std::vector<std::pair<bool, GraphEdge<M> *>> tmp_vec = {tmp};
        nodes.push_back(new GraphNodeAdjStr<T, M>(data));
        if (edges.empty()) {
            edges.emplace_back(tmp_vec);
        } else {
            edges.emplace_back();
            for (unsigned int i = 0; i < edges.size(); i++) {
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
        if (!directed && index1 != index2) {
            edges[index2][index1].first = true;
            edges[index2][index1].second = edge;
        }
    }

    int find_node(T key) override {
        for (unsigned int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->data == key) return (int) i;
        }
        return -1;
    }
};

void test_int(Graph<int, double> *graph) {
    for (int i = 0; i < 10; i++) {
        graph->add_node(i * 2);
    }
    graph->add_edge(0, 1, 0);
    graph->add_edge(1, 2, 0);
    graph->add_edge(2, 3, 0);
    graph->add_edge(3, 4, 0);
    graph->add_edge(4, 5, 0);
    graph->add_edge(5, 6, 0);
    graph->add_edge(6, 7, 0);
    graph->add_edge(7, 8, 0);
    graph->add_edge(8, 9, 0);
    graph->print();
    std::cout << "res:: " << graph->adjacent(0) << std::endl;
    std::cout << "find 3: " << graph->find_node(3) << std::endl;
    std::cout << "find 2: " << graph->find_node(2) << std::endl;
    std::cout << "find 6: " << graph->find_node(6) << std::endl;
    std::cout << "find 5: " << graph->find_node(5) << std::endl;
    std::cout << "find 10: " << graph->find_node(10) << std::endl;
    std::cout << "find 8: " << graph->find_node(8) << std::endl;
    std::cout << std::endl << std::endl;
}


int main() {
    auto G1 = new GraphAdjStr<int, double>();
    test_int(G1);
    auto G2 = new GraphMtrx<int, double>();
    test_int(G2);

    return 0;
}