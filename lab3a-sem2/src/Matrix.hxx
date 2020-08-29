#pragma once

#include <iostream>
#include <cassert>
#include "Matrix.hpp"

namespace detail {
    template<typename TT>
    Matrix<TT> defaultMultiplication(const Matrix<TT> &lhs, const Matrix<TT> &rhs) {
        assert(lhs.size_cols() == rhs.size_rows() && "Incorrect");

        Matrix res(lhs.size_rows(), rhs.size_cols(), 0);

        for (std::size_t i = 0; i < lhs.size_rows(); i++)
            for (std::size_t j = 0; j < rhs.size_cols(); j++)
                for (std::size_t k = 0; k < lhs.size_cols(); k++)
                    res(i, j) += lhs(i, k) * rhs(k, j);

        return res;
    }
}

template<typename T>
Matrix<T>::Matrix(const std::size_t &rows, const std::size_t &cols, const T &default_element) :
        _rows_data(rows, std::vector(cols, default_element)) {}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::vector<T>> elements) :
        _rows_data{elements} {
    assert(elements.size() != 0 && "No elements provided");
    auto size_tmp = _rows_data[0].size();
    for (std::size_t i = 1; i < _rows_data.size(); i++) {
        assert(_rows_data[i].size() == size_tmp && "Wrong elements sequence provided");
    }
}

template<typename T>
std::size_t Matrix<T>::size_rows() const {
    return _rows_data.size();
}

template<typename T>
std::size_t Matrix<T>::size_cols() const {
    return _rows_data.empty() ? 0 : _rows_data[0].size();
}

template<typename T>
T Matrix<T>::operator()(const std::size_t &row, const std::size_t &col) const {
    assert(row < size_rows() && "Out of range");
    assert(col < size_cols() && "Out of range");
    return _rows_data[row][col];
}

template<typename T>
T &Matrix<T>::operator()(const std::size_t &row, const std::size_t &col) {
    assert(row < size_rows() && "Out of range");
    assert(col < size_cols() && "Out of range");
    return _rows_data[row][col];
}

template<typename T>
Matrix<T> operator+(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    assert(lhs.size_rows() == rhs.size_rows() && "Incorrect arguments");
    assert(lhs.size_cols() == rhs.size_cols() && "Incorrect arguments");

    Matrix res(lhs.size_rows(), lhs.size_cols(), 0);

    for (std::size_t i = 0; i < lhs.size_rows(); i++) {
        for (int j = 0; j < lhs.size_cols(); j++) {
            res(i, j) = lhs(i, j) + rhs(i, j);
        }
    }

    return res;
}

template<typename T>
Matrix<T> operator-(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    assert(lhs.size_rows() == rhs.size_rows() && "Incorrect arguments");
    assert(lhs.size_cols() == rhs.size_cols() && "Incorrect arguments");

    Matrix res(lhs.size_rows(), lhs.size_cols(), 0);

    for (std::size_t i = 0; i < lhs.size_rows(); i++) {
        for (int j = 0; j < lhs.size_cols(); j++) {
            res(i, j) = lhs(i, j) - rhs(i, j);
        }
    }

    return res;
}

template<typename T>
bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    if (lhs.size_rows() != rhs.size_rows() || lhs.size_cols() != rhs.size_cols()) return false;

    for(int i = 0; i < lhs.size_rows(); i++){
        for(int j = 0; j < lhs.size_cols(); j++){
            if (lhs(i, j) != rhs(i, j)) return false;
        }
    }

    return true;
}

template<typename T>
bool operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    return !(rhs == lhs);
}

template<typename T>
template<typename OStream>
void Matrix<T>::print(OStream& os) {
    for (std::size_t i = 0; i < size_rows(); i++) {
        for (std::size_t j = 0; j < size_cols(); j++) {
            os << _rows_data[i][j] << ' ';
        }
        os << std::endl;
    }
}



template<typename T>
Matrix<T> multiply(const Matrix<T> &lhs, const Matrix<T> &rhs,  MatrixMultiplicationPolicy policy) {
    switch (policy) {
        case MatrixMultiplicationPolicy::Default:
            return detail::defaultMultiplication(std::move(lhs), std::move(rhs));
        case MatrixMultiplicationPolicy::Strassen:
            return {{0}};
        case MatrixMultiplicationPolicy::StrassenParallel:
            return {{0}};
    }

    return {{0}};
}