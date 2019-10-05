#include "graph.h"
#include "graph_adj_str.h"
#include "graph_mtrx.h"
#include "dice.h"
#include "dice_set.h"

#include <iostream>
#include <vector>
#include <string>

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
//    auto G1 = new GraphAdjStr<int, double>();
//    test_int(G1);
//    auto G2 = new GraphMtrx<int, double>();
//    test_int(G2);
    DiceSet set({{2, {0.3, 0.7}},
                 {3, {0.2, 0.3, 0.5}},
                 {2, {0.1, 0.9}}});
    set.print_sum_probabilities();
    std::cout << "expected value: " << set.expected_value();
    return 0;
}