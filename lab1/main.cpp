#include <iostream>
#include <vector>

template<typename T, typename M>
class Graph {
public:
    virtual void add_node(T data) = 0;
    virtual bool check_nodes_adjacency(int index1, int index2) = 0;

    //virtual void add_edge(int index1, int index2, M data) = 0;
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

    bool check_nodes_adjacency(int index1, int index2) override{
        for(auto &e:nodes[index1]->adjacent_nodes){
            if (e.first == index2){
                return true;
            }
        }
        return false;
    }

    void add_node(T data) override {
        nodes.push_back(new GraphNodeAdjStr<T, M>(data));
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

    bool check_nodes_adjacency(int index1, int index2) override{
        for(auto &edge:edges[index1]){
            if (edge.first == index2){
                return true;
            }
        }
        return false;
    }

    void add_node(T data) override {
        static const std::pair<bool, GraphEdge<M> *> tmp = {false, nullptr};
        nodes.push_back(new GraphNodeAdjStr<T, M>(data));
        if (edges.empty()) {
            edges.emplace_back(tmp);
        } else {
            edges.emplace_back();
            for (int i = 0; i < edges.size(); i ++){
                edges.emplace_back(tmp);
                edges[edges.size()-1].emplace_back(tmp);
            }
        }
    }
};

void test_int(Graph<int, int> *graph) {
    graph->add_node(5);
}


int main() {
    auto G1 = new GraphAdjStr<int, int>();
    test_int(G1);
    auto G2 = new GraphMtrx<int, int>();
    test_int(G2);

    return 0;
}