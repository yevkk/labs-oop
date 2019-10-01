#ifndef LAB1_GRAPH_MTRX_HXX
#define LAB1_GRAPH_MTRX_HXX

#include "graph_mtrx.h"
#include "infnull.h"
#include <iostream>
#include <vector>


template<typename NodeDataT, typename EdgeDataT>
GraphMtrx<NodeDataT, EdgeDataT>::GraphMtrx(bool Directed) {
    directed = Directed;
}

template<typename NodeDataT, typename EdgeDataT>
GraphMtrx<NodeDataT, EdgeDataT>::~GraphMtrx() {
    for (auto &node:nodes) delete node;
    for (auto &edge:edges)
        for (int i = 0; i < edge.size(); i++)
            delete edge[i].second;
}

template<typename NodeDataT, typename EdgeDataT>
void GraphMtrx<NodeDataT, EdgeDataT>::print() {
    std::cout << "========== [ Graph | " << "directed: " << std::boolalpha << directed << " ] =========="
              << std::endl;

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

template<typename NodeDataT, typename EdgeDataT>
bool GraphMtrx<NodeDataT, EdgeDataT>::nodes_adjacency(int index1, int index2) {
    if (index1 < nodes.size() && index2 < nodes.size()) {
        return edges[index1][index2].first;
    } else return false;
}

template<typename NodeDataT, typename EdgeDataT>
bool GraphMtrx<NodeDataT, EdgeDataT>::adjacent(int index) {
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

template<typename NodeDataT, typename EdgeDataT>
void GraphMtrx<NodeDataT, EdgeDataT>::add_node(NodeDataT data) {
    static const std::pair<bool, GraphEdge<EdgeDataT> *> tmp = {false, nullptr};
    static const std::vector<std::pair<bool, GraphEdge<EdgeDataT> *>> tmp_vec = {tmp};
    nodes.push_back(new GraphNodeAdjStr<NodeDataT, EdgeDataT>(data));
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

template<typename NodeDataT, typename EdgeDataT>
void GraphMtrx<NodeDataT, EdgeDataT>::add_edge(int index1, int index2, EdgeDataT data) {
    if (!(index1 < nodes.size() && index2 < nodes.size())) return;

    auto edge = new GraphEdge<EdgeDataT>(data);
    edges[index1][index2].first = true;
    edges[index1][index2].second = edge;
    if (!directed && index1 != index2) {
        edges[index2][index1].first = true;
        edges[index2][index1].second = edge;
    }
}

template<typename NodeDataT, typename EdgeDataT>
int GraphMtrx<NodeDataT, EdgeDataT>::find_node(NodeDataT key) {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (nodes[i]->data == key) return (int) i;
    }
    return -1;
}

template<typename NodeDataT, typename EdgeDataT>
bool GraphMtrx<NodeDataT, EdgeDataT>::delete_node_by_index(int index) {
    if (index >= nodes.size()) return false;

    int i = 0;
    for (auto &e:edges) {
        delete e[index].second;
        e.erase(e.begin() + index);
    }
    for (auto &e:edges[index]) {
        delete e.second;
    }
    edges.erase(edges.begin() + index);
    nodes.erase(nodes.begin() + index);
    return true;
}

template<typename NodeDataT, typename EdgeDataT>
bool GraphMtrx<NodeDataT, EdgeDataT>::delete_node_by_value(NodeDataT key) {
    int i = -2;
    while ((i = find_node(key)) != -1) {
        delete_node_by_index(i);
    }
    return i != -2;
}

template<typename NodeDataT, typename EdgeDataT>
bool GraphMtrx<NodeDataT, EdgeDataT>::delete_edge(int index1, int index2) {
    if (!(index1 < nodes.size() && index2 < nodes.size())) return false;

    edges[index1][index2].first = false;
    delete edges[index1][index2].second;
    edges[index1][index2].second = nullptr;

    if (!directed) {
        edges[index2][index1].first = false;
        delete edges[index2][index1].second;
        edges[index2][index1].second = nullptr;
    }
    return true;
}

template<typename NodeDataT, typename EdgeDataT>
EdgeDataT GraphMtrx<NodeDataT, EdgeDataT>::min_distance(int index1, int index2) {
    if (!(index1 < nodes.size() && index2 < nodes.size())) return -1;
    std::vector<std::vector<EdgeDataT>> dist;
    std::vector<EdgeDataT> tmp;

    EdgeDataT m_null = null<EdgeDataT>::value;

    for (auto &e:nodes) {
        tmp.push_back(m_null);
    }

    for (auto &e:nodes) {
        dist.push_back(tmp);
    }

    for (unsigned int i = 0; i < nodes.size(); i++) {
        for (unsigned int j = 0; j < nodes.size(); j++) {
            if (i == j) dist[i][j] = m_null;
            else if (edges[i][j].first) dist[i][j] = edges[i][j].second->data;
            else dist[i][j] = inf<EdgeDataT>::value;
        }
    }

    for (unsigned int k = 0; k < nodes.size(); k++) {
        for (unsigned int i = 0; i < nodes.size(); i++) {
            for (unsigned int j = 0; j < nodes.size(); j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    return dist[index1][index2];
}

#endif //LAB1_GRAPH_MTRX_HXX
