#ifndef INFNULL_H
#define INFNULL_H

#include <vector>
#include <string>

template<typename T>
struct inf;

template<>
struct inf<int> {
    static constexpr int value = INT_MAX;
};

template<>
struct inf<double> {
    static constexpr double value = INT_MAX;
};

template<>
struct inf<std::string> {
    const std::string value = {CHAR_MAX};
};

template<typename T>
struct inf<std::vector<T>> {
    static constexpr std::vector<T> value = {inf<T>::value};
};

template<typename T>
struct null;

template<>
struct null<int> {
    static constexpr int value = 0;
};

template<>
struct null<double> {
    static constexpr double value = 0;
};

template<>
struct null<std::string> {
    const std::string value = "";
};

template<typename T>
struct null<std::vector<T>> {
    static constexpr std::vector<T> value = {};
};

#endif //INFNULL_H
