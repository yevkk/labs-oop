#ifndef LAB1_GRAPH_ADJ_STR_HXX
#define LAB1_GRAPH_ADJ_STR_HXX

#include "graph_adj_str.h"
#include "infnull.h"
#include <iostream>
#include <vector>

template<typename NodeDataT, typename EdgeDataType>
GraphNodeAdjStr<NodeDataT, EdgeDataType>::GraphNodeAdjStr(int Data) {
    this->data = Data;
}

template<typename NodeDataT, typename EdgeDataType>
GraphAdjStr<NodeDataT, EdgeDataType>::GraphAdjStr(bool Directed) {
    directed = Directed;
}

template<typename NodeDataT, typename EdgeDataType>
GraphAdjStr<NodeDataT, EdgeDataType>::~GraphAdjStr() {
    for (auto &node:nodes) delete node;
}

template<typename NodeDataT, typename EdgeDataType>
void GraphAdjStr<NodeDataT, EdgeDataType>::print() {
    std::cout << "========== [ Graph | " << "directed: " << std::boolalpha << directed << " ] =========="
              << std::endl;

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

template<typename NodeDataT, typename EdgeDataType>
bool GraphAdjStr<NodeDataT, EdgeDataType>::nodes_adjacency(int index1, int index2) {
    for (auto &e:nodes[index1]->adjacent_nodes) {
        if (e.first == index2) {
            return true;
        }
    }
    return false;
}

template<typename NodeDataT, typename EdgeDataType>
bool GraphAdjStr<NodeDataT, EdgeDataType>::adjacent(int index) {
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

template<typename NodeDataT, typename EdgeDataType>
void GraphAdjStr<NodeDataT, EdgeDataType>::add_node(NodeDataT data) {
    nodes.push_back(new GraphNodeAdjStr<NodeDataT, EdgeDataType>(data));
}

template<typename NodeDataT, typename EdgeDataType>
void GraphAdjStr<NodeDataT, EdgeDataType>::add_edge(int index1, int index2, EdgeDataType data) {
    if (!(index1 < nodes.size() && index2 < nodes.size())) return;

    auto edge = new GraphEdge<EdgeDataType>(data);
    nodes[index1]->adjacent_nodes.emplace_back(index2, edge);
    if (!directed && index1 != index2)
        nodes[index2]->adjacent_nodes.emplace_back(index1, edge);
}

template<typename NodeDataT, typename EdgeDataType>
int GraphAdjStr<NodeDataT, EdgeDataType>::find_node(NodeDataT key) {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (nodes[i]->data == key) return (int) i;
    }
    return -1;
}

template<typename NodeDataT, typename EdgeDataType>
bool GraphAdjStr<NodeDataT, EdgeDataType>::delete_node_by_index(int index) {
    if (index >= nodes.size()) return false;

    int i = 0;
    for (auto &e:nodes) {
        while (i < e->adjacent_nodes.size()) {
            if (e->adjacent_nodes[i].first == index) {
                delete e->adjacent_nodes[i].second;
                e->adjacent_nodes.erase(e->adjacent_nodes.begin() + i);
            } else {
                if (e->adjacent_nodes[i].first > index) e->adjacent_nodes[i].first--;
                i++;
            }
        }
        i = 0;
    }
    for (auto &e:nodes[index]->adjacent_nodes) {
        delete e.second;
    }
    nodes.erase(nodes.begin() + index);
    return true;
}

template<typename NodeDataT, typename EdgeDataType>
bool GraphAdjStr<NodeDataT, EdgeDataType>::delete_node_by_value(NodeDataT key) {
    int i = -2;
    while ((i = find_node(key)) != -1) {
        delete_node_by_index(i);
    }
    return i != -2;
}

template<typename NodeDataT, typename EdgeDataType>
bool GraphAdjStr<NodeDataT, EdgeDataType>::delete_edge(int index1, int index2) {
    if (!(index1 < nodes.size() && index2 < nodes.size())) return false;
    static bool flag = true;
    for (unsigned int i = 0; i < nodes[index1]->adjacent_nodes.size(); i++) {
        if (nodes[index1]->adjacent_nodes[i].first == index2) {
            delete nodes[index1]->adjacent_nodes[i].second;
            nodes[index1]->adjacent_nodes.erase(nodes[index1]->adjacent_nodes.begin() + i);
            if (flag) {
                flag = false;
                if (!directed) delete_edge(index2, index1);
                flag = true;
                return true;
            }
        }
    }
    return false;
}


template<typename NodeDataT, typename EdgeDataType>
EdgeDataType GraphAdjStr<NodeDataT, EdgeDataType>::min_distance(int index1, int index2) {
    if (!(index1 < nodes.size() && index2 < nodes.size())) return -1;
    std::vector<std::vector<EdgeDataType>> dist;
    std::vector<EdgeDataType> tmp;

    EdgeDataType m_null = null<EdgeDataType>::value;

    for (auto &e:nodes) {
        tmp.push_back(m_null);
    }

    for (auto &e:nodes) {
        dist.push_back(tmp);
    }

    for (unsigned int i = 0; i < nodes.size(); i++) {
        for (auto &e:nodes[i]->adjacent_nodes) {
            dist[i][e.first] = e.second->data;
        }
    }

    for (unsigned int i = 0; i < nodes.size(); i++) {
        for (unsigned int j = 0; j < nodes.size(); j++) {
            if ((i != j) && (dist[i][j] == m_null)) dist[i][j] = inf<EdgeDataType>::value;
            if (i == j) dist[i][j] = m_null;
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

//IMPORTANT NOTE: WORKS CORRECTLY ONLY FOR DIRECTED GRAPHS!
template<typename NodeDataT, typename EdgeDataType>
bool GraphAdjStr<NodeDataT, EdgeDataType>::cycle_exist(int index, bool start) {
    static std::vector<bool> visited;
    if (start) {
        visited.clear();
        for (auto &n:nodes) visited.emplace_back(false);
    }

    if (visited[index]) return true;
    visited[index] = true;
    for (auto &e:nodes[index]->adjacent_nodes) {
        if (e.first != index)
            if (cycle_exist(e.first, false)) return true;
    }
    return false;
}

#endif //LAB1_GRAPH_ADJ_STR_HXX
