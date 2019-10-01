#include "infnull.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

template<typename T, typename M>
class Graph {
public:
    virtual void print() = 0;

    virtual bool nodes_adjacency(int index1, int index2) = 0;

    virtual bool adjacent(int index) = 0;

    virtual void add_node(T data) = 0;

    virtual void add_edge(int index1, int index2, M data) = 0;

    virtual int find_node(T key) = 0; //returns index of first element equal to key or -1 if there is no such element;

    virtual bool delete_node_by_index(int index) = 0;

    virtual bool delete_node_by_value(T key) = 0;

    virtual bool delete_edge(int index1, int index2) = 0;

    virtual M min_distance(int index1, int index2) = 0; //based on Floyd's algorithm;
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
private:
    std::vector<GraphNodeAdjStr<T, M> *> nodes;

public:
    bool directed;

    explicit GraphAdjStr(bool Directed = false) {
        directed = Directed;
    }

    ~GraphAdjStr() {
        for (auto &node:nodes) delete node;
    }

    void print() override {
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

    bool delete_node_by_index(int index) override {
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

    bool delete_node_by_value(T key) override {
        int i = -2;
        while ((i = find_node(key)) != -1) {
            delete_node_by_index(i);
        }
        return i != -2;
    }

    bool delete_edge(int index1, int index2) override {
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

    M min_distance(int index1, int index2) override {
        if (!(index1 < nodes.size() && index2 < nodes.size())) return -1;
        std::vector<std::vector<M>> dist;
        std::vector<M> tmp;

        M m_null = null<M>::value;

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
                if ((i != j) && (dist[i][j] == m_null)) dist[i][j] = inf<M>::value;
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
};

template<typename T, typename M>
class GraphMtrx : public Graph<T, M> {
private:
    std::vector<GraphNode<T> *> nodes;
    std::vector<std::vector<std::pair<bool, GraphEdge<M> *>>> edges;

public:
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

    bool delete_node_by_index(int index) override {
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

    bool delete_node_by_value(T key) override {
        int i = -2;
        while ((i = find_node(key)) != -1) {
            delete_node_by_index(i);
        }
        return i != -2;
    }

    bool delete_edge(int index1, int index2) override {
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

    M min_distance(int index1, int index2) override {
        if (!(index1 < nodes.size() && index2 < nodes.size())) return -1;
        std::vector<std::vector<M>> dist;
        std::vector<M> tmp;

        M m_null = null<M>::value;

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
                else dist[i][j] = inf<M>::value;
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

};

class Dice {
public:
    int face_number;
    double *probability; //i-th element is a probability of dropping face with number (i+1)

    Dice(int Face_number, const double Probability[]) {
        double tmp = 1;
        face_number = Face_number;
        probability = new double[face_number];
        for (int i = 0; i < face_number; i++) {
            if (Probability[i] < tmp) {
                probability[i] = Probability[i];
                tmp -= Probability[i];
            } else {
                probability[i] = tmp;
                tmp = 0;
            }

        }
    }
};

class DiceSet {
public:
    std::vector<Dice> elements;

    DiceSet() = default;

    int min_sum() {
        return elements.size();
    }

        int max_sum() {
            int res = 0;
            for (auto &e:elements)
                res += e.face_number;
            return res;
}

};

template<typename T>
bool operator<(std::vector<T> &vec1, std::vector<T> &vec2) {
    int i = 0;
    while (vec1[i] == vec2[i]) {
        i++;
    }
    return vec1[i] < vec2[i];
}

template<typename T>
bool operator>(std::vector<T> &vec1, std::vector<T> &vec2) {
    return vec2 < vec1;
}

template<typename T>
bool operator<=(std::vector<T> &vec1, std::vector<T> &vec2) {
    return !(vec1 > vec2);
}

template<typename T>
bool operator>=(std::vector<T> &vec1, std::vector<T> &vec2) {
    return !(vec1 < vec2);
}

template<typename T>
bool operator==(std::vector<T> &vec1, std::vector<T> &vec2) {
    return !(vec1 < vec2) && !(vec1 > vec2);
}

template<typename T>
bool operator!=(std::vector<T> &vec1, std::vector<T> &vec2) {
    return !(vec1 == vec2);
}

template<typename T>
std::vector<T> operator+(std::vector<T> &vec1, std::vector<T> &vec2) {
    std::vector<T> res;
    int size = (vec1.size() < vec2.size()) ? vec2.size() : vec1.size();
    res.reserve(size);
    for (int i = 0; i < size; i++) {
        res.emplace_back(vec1[i] + vec2[i]);
    }
    return res;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> &vec) {
    for (auto &e:vec) {
        os << e << ' ';
    }
    os << std::endl;
    return os;
}

void test_int(Graph<int, double> *graph) {
    for (int i = 0; i < 5; i++) {
        graph->add_node(i * 2);
    }
    graph->add_edge(0, 1, 1);
    graph->add_edge(0, 4, 5);
    graph->add_edge(1, 4, 3);
    graph->add_edge(0, 2, 1);
    graph->add_edge(2, 3, 1);
    graph->add_edge(3, 4, 1);

    graph->print();

    std::cout << graph->min_distance(0, 4);
    std::cout << graph->min_distance(4, 2);
    std::cout << std::endl << std::endl;
}


int main() {
    auto G1 = new GraphAdjStr<int, double>();
    test_int(G1);
//    auto G2 = new GraphMtrx<int, double>();
//    test_int(G2);
    return 0;
}