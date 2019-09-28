#include <iostream>
#include <vector>

template<typename T, typename M>
class Graph {
public:
};

template <typename T>
class GraphEdge {
public:
    T data;

    explicit GraphEdge(const T Data) {
        data = Data;
    }
};

template <typename T>
class GraphNode {
public:
    T data;

    GraphNode() = default;

    explicit GraphNode(const T Data) {
        data = Data;
    }
};


template<typename T, typename M>
class GraphNodeAdjStr : public GraphNode<T>{
public:
    std::vector<std::pair<int, GraphEdge<M>*>> adjacent_nodes;

    explicit GraphNodeAdjStr(int Data){
        this->data = Data;
    }
};

template<typename T, typename M>
class GraphAdjStr : public Graph<T, M> {
    std::vector<GraphNodeAdjStr<T, M> *> nodes;
    bool directed;

    explicit GraphAdjStr(bool Directed = false) {
        directed = Directed;
    }

    ~GraphAdjStr() {
        for (auto &node:nodes) delete node;
    }
};

template<typename T, typename M>
class GraphMtrx : public Graph<T, M>{
    std::vector<GraphNode<T>*> nodes;
    std::vector<std::vector<std::pair<bool, GraphEdge<M>*>>> edges;
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
};


int main() {


    return 0;
}