#ifndef GRAPH_ADJ_STR_H
#define GRAPH_ADJ_STR_H

#include "graph.h"
#include <iostream>
#include <vector>

template<typename NodeDataT, typename EdgeDataT>
class GraphNodeAdjStr : public GraphNode<NodeDataT> {
public:
    std::vector<std::pair<int, GraphEdge<EdgeDataT> *>> adjacent_nodes;

    explicit GraphNodeAdjStr(int Data);
};

template<typename NodeDataT, typename EdgeDataT>
class GraphAdjStr : public Graph<NodeDataT, EdgeDataT> {
private:
    std::vector<GraphNodeAdjStr<NodeDataT, EdgeDataT> *> nodes;

public:
    bool directed;

    explicit GraphAdjStr(bool Directed = false);

    ~GraphAdjStr();

    void print() override;

    bool nodes_adjacency(int index1, int index2) override;

    bool adjacent(int index) override;

    void add_node(NodeDataT data) override;

    void add_edge(int index1, int index2, EdgeDataT data) override;

    int find_node(NodeDataT key) override;

    bool delete_node_by_index(int index) override;

    bool delete_node_by_value(NodeDataT key) override;

    bool delete_edge(int index1, int index2) override;

    EdgeDataT min_distance(int index1, int index2) override;

    //testing methods for another lab
    bool cycle_exist_step(int index, std::vector<bool> visited, std::vector<bool> &rec_stack);
    bool cycle_exist();

};

#include "graph_adj_str.hxx"

#endif //GRAPH_ADJ_STR_H
