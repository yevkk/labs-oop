#ifndef LAB1_GRAPH_H
#define LAB1_GRAPH_H

template<typename NodeDataT, typename EdgeDataT>
class Graph {
public:
    virtual void print() = 0;

    virtual bool nodes_adjacency(int index1, int index2) = 0;

    virtual bool adjacent(int index) = 0;

    virtual void add_node(NodeDataT data) = 0;

    virtual void add_edge(int index1, int index2, EdgeDataT data) = 0;

    virtual int find_node(NodeDataT key) = 0; //returns index of first element equal to key or -1 if there is no such element;

    virtual bool delete_node_by_index(int index) = 0;

    virtual bool delete_node_by_value(NodeDataT key) = 0;

    virtual bool delete_edge(int index1, int index2) = 0;

    virtual EdgeDataT min_distance(int index1, int index2) = 0; //based on Floyd's algorithm;
};

template<typename EdgeDataT>
class GraphEdge {
public:
    EdgeDataT data;

    explicit GraphEdge(const EdgeDataT Data) {
        data = Data;
    }
};

template<typename NodeDataT>
class GraphNode {
public:
    NodeDataT data;

    GraphNode() = default;

    explicit GraphNode(const NodeDataT Data) {
        data = Data;
    }
};

#endif //LAB1_GRAPH_H
