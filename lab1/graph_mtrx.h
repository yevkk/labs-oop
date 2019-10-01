#ifndef LAB1_GRAPH_MTRX_H
#define LAB1_GRAPH_MTRX_H

#include "graph.h"
#include <iostream>
#include <vector>

template<typename NodeDataT, typename EdgeDataT>
class GraphMtrx : public Graph<NodeDataT, EdgeDataT> {
private:
    std::vector<GraphNode<NodeDataT> *> nodes;
    std::vector<std::vector<std::pair<bool, GraphEdge<EdgeDataT> *>>> edges;

public:
    bool directed;

    explicit GraphMtrx(bool Directed = false);

    ~GraphMtrx();

    void print() override;

    bool nodes_adjacency(int index1, int index2);

    bool adjacent(int index) override;

    void add_node(NodeDataT data) override;

    void add_edge(int index1, int index2, EdgeDataT data) override;

    int find_node(NodeDataT key) override;

    bool delete_node_by_index(int index) override;

    bool delete_node_by_value(NodeDataT key) override;

    bool delete_edge(int index1, int index2) override;

    EdgeDataT min_distance(int index1, int index2) override;
};

#include "graph_mtrx.hxx"

#endif //LAB1_GRAPH_MTRX_H
