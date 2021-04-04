#pragma once

#include "Matrix.hpp"

#include <iostream>
#include <cassert>
#include <algorithm>
#include <future>

namespace detail {
    template<typename TT>
    Matrix<TT> defaultMultiplication(const Matrix<TT> &lhs, const Matrix<TT> &rhs) {
        Matrix res(lhs.size_rows(), rhs.size_cols(), 0);

        for (std::size_t i = 0; i < lhs.size_rows(); i++)
            for (std::size_t j = 0; j < rhs.size_cols(); j++)
                for (std::size_t k = 0; k < lhs.size_cols(); k++)
                    res(i, j) += lhs(i, k) * rhs(k, j);

        return res;
    }

    template<typename TT>
    Matrix<TT> StrassenMultiplication(const Matrix<TT> &lhs,
                                      const Matrix<TT> &rhs,
                                      MatrixMultiplicationPolicy policy,
                                      const std::size_t &size_bound) {
        std::size_t sqr_size = std::max({lhs.size_rows(), lhs.size_cols(), rhs.size_rows(), rhs.size_cols()});
        std::size_t power2 = 1;
        while (power2 < sqr_size) {
            power2 *= 2;
        }

        Matrix<TT> new_lhs(power2, power2, 0);
        Matrix<TT> new_rhs(power2, power2, 0);

        for (std::size_t i = 0; i < lhs.size_rows(); i++) {
            std::copy(lhs._rows[i].begin(), lhs._rows[i].end(), new_lhs._rows[i].begin());
        }

        for (std::size_t i = 0; i < rhs.size_rows(); i++) {
            std::copy(rhs._rows[i].begin(), rhs._rows[i].end(), new_rhs._rows[i].begin());
        }

        Matrix<TT> sub_res{{0}};
        if (policy == MatrixMultiplicationPolicy::Strassen) {
            sub_res = StrassenMultiplicationStep(new_lhs, new_rhs, power2, size_bound);
        } else {
            sub_res = StrassenMultiplicationStepParallel(new_lhs, new_rhs, power2, size_bound);
        }

        Matrix res(lhs.size_rows(), rhs.size_cols(), 0);
        for (std::size_t i = 0; i < lhs.size_rows(); i++) {
            std::copy(sub_res._rows[i].begin(),
                      sub_res._rows[i].begin() + res.size_cols(),
                      res._rows[i].begin()
            );
        }

        return res;
    }

    template<typename TT>
    Matrix<TT> StrassenMultiplicationStep(const Matrix<TT> &lhs,
                                          const Matrix<TT> &rhs,
                                          const std::size_t &size,
                                          const std::size_t &size_bound) {
        if (size <= size_bound) {
            return defaultMultiplication(lhs, rhs);
        }

        std::size_t half_size = size / 2;

        Matrix<TT> A11(half_size, half_size, 0);
        Matrix<TT> A12(half_size, half_size, 0);
        Matrix<TT> A21(half_size, half_size, 0);
        Matrix<TT> A22(half_size, half_size, 0);
        Matrix<TT> B11(half_size, half_size, 0);
        Matrix<TT> B12(half_size, half_size, 0);
        Matrix<TT> B21(half_size, half_size, 0);
        Matrix<TT> B22(half_size, half_size, 0);

        for (int i = 0; i < size / 2; i++) {
            std::copy(lhs._rows[i].begin(),
                      lhs._rows[i].begin() + half_size,
                      A11._rows[i].begin()
            );
            std::copy(lhs._rows[i].begin() + half_size,
                      lhs._rows[i].end(),
                      A12._rows[i].begin()
            );
            std::copy(lhs._rows[i + half_size].begin(),
                      lhs._rows[i + half_size].begin() + half_size,
                      A21._rows[i].begin()
            );
            std::copy(lhs._rows[i + half_size].begin() + half_size,
                      lhs._rows[i + half_size].end(),
                      A22._rows[i].begin()
            );

            std::copy(rhs._rows[i].begin(),
                      rhs._rows[i].begin() + half_size,
                      B11._rows[i].begin()
            );
            std::copy(rhs._rows[i].begin() + half_size,
                      rhs._rows[i].end(),
                      B12._rows[i].begin()
            );
            std::copy(rhs._rows[i + half_size].begin(),
                      rhs._rows[i + half_size].begin() + half_size,
                      B21._rows[i].begin()
            );
            std::copy(rhs._rows[i + half_size].begin() + half_size,
                      rhs._rows[i + half_size].end(),
                      B22._rows[i].begin()
            );
        }

        auto P = StrassenMultiplicationStep(A11 + A22, B11 + B22, half_size, size_bound);
        auto Q = StrassenMultiplicationStep(A21 + A22, B11, half_size, size_bound);
        auto R = StrassenMultiplicationStep(A11, B12 - B22, half_size, size_bound);
        auto S = StrassenMultiplicationStep(A22, B21 - B11, half_size, size_bound);
        auto T = StrassenMultiplicationStep(A11 + A12, B22, half_size, size_bound);
        auto U = StrassenMultiplicationStep(A21 - A11, B11 + B12, half_size, size_bound);
        auto V = StrassenMultiplicationStep(A12 - A22, B21 + B22, half_size, size_bound);

        A11 = P + S - T + V;
        A12 = R + T;
        A21 = Q + S;
        A22 = P + R - Q + U;

        Matrix res(size, size, 0);
        for (int i = 0; i < size / 2; i++) {
            std::move(A11._rows[i].begin(), A11._rows[i].end(), res._rows[i].begin());
            std::move(A12._rows[i].begin(), A12._rows[i].end(), res._rows[i].begin() + half_size);
            std::move(A21._rows[i].begin(), A21._rows[i].end(), res._rows[i + half_size].begin());
            std::move(A22._rows[i].begin(), A22._rows[i].end(), res._rows[i + half_size].begin() + half_size);
        }

        return res;
    }

    template<typename TT>
    Matrix<TT> StrassenMultiplicationStepParallel(const Matrix<TT> &lhs,
                                                  const Matrix<TT> &rhs,
                                                  const std::size_t &size,
                                                  const std::size_t &size_bound) {
        if (size <= size_bound) {
            return defaultMultiplication(lhs, rhs);
        }

        std::size_t half_size = size / 2;

        Matrix<TT> A11(half_size, half_size, 0);
        Matrix<TT> A12(half_size, half_size, 0);
        Matrix<TT> A21(half_size, half_size, 0);
        Matrix<TT> A22(half_size, half_size, 0);
        Matrix<TT> B11(half_size, half_size, 0);
        Matrix<TT> B12(half_size, half_size, 0);
        Matrix<TT> B21(half_size, half_size, 0);
        Matrix<TT> B22(half_size, half_size, 0);

        for (int i = 0; i < size / 2; i++) {
            std::copy(lhs._rows[i].begin(),
                      lhs._rows[i].begin() + half_size,
                      A11._rows[i].begin()
            );
            std::copy(lhs._rows[i].begin() + half_size,
                      lhs._rows[i].end(),
                      A12._rows[i].begin()
            );
            std::copy(lhs._rows[i + half_size].begin(),
                      lhs._rows[i + half_size].begin() + half_size,
                      A21._rows[i].begin()
            );
            std::copy(lhs._rows[i + half_size].begin() + half_size,
                      lhs._rows[i + half_size].end(),
                      A22._rows[i].begin()
            );

            std::copy(rhs._rows[i].begin(),
                      rhs._rows[i].begin() + half_size,
                      B11._rows[i].begin()
            );
            std::copy(rhs._rows[i].begin() + half_size,
                      rhs._rows[i].end(),
                      B12._rows[i].begin()
            );
            std::copy(rhs._rows[i + half_size].begin(),
                      rhs._rows[i + half_size].begin() + half_size,
                      B21._rows[i].begin()
            );
            std::copy(rhs._rows[i + half_size].begin() + half_size,
                      rhs._rows[i + half_size].end(),
                      B22._rows[i].begin()
            );
        }

        auto P_fut = std::async(std::launch::async,
                                StrassenMultiplicationStepParallel<TT>,
                                A11 + A22, B11 + B22, half_size, size_bound
        );
        auto Q_fut = std::async(std::launch::async,
                                StrassenMultiplicationStepParallel<TT>,
                                A21 + A22, B11, half_size, size_bound
        );
        auto R_fut = std::async(std::launch::async,
                               StrassenMultiplicationStepParallel<TT>,
                                A11, B12 - B22, half_size, size_bound
        );

        P_fut.wait();
        Q_fut.wait();
        R_fut.wait();

        auto P = P_fut.get();
        auto Q = Q_fut.get();
        auto R = R_fut.get();
        auto S = StrassenMultiplicationStepParallel(A22, B21 - B11, half_size, size_bound);
        auto T = StrassenMultiplicationStepParallel(A11 + A12, B22, half_size, size_bound);
        auto U = StrassenMultiplicationStepParallel(A21 - A11, B11 + B12, half_size, size_bound);
        auto V = StrassenMultiplicationStepParallel(A12 - A22, B21 + B22, half_size, size_bound);


        A11 = P + S - T + V;
        A12 = R + T;
        A21 = Q + S;
        A22 = P + R - Q + U;

        Matrix res(size, size, 0);
        for (int i = 0; i < size / 2; i++) {
            std::move(A11._rows[i].begin(), A11._rows[i].end(), res._rows[i].begin());
            std::move(A12._rows[i].begin(), A12._rows[i].end(), res._rows[i].begin() + half_size);
            std::move(A21._rows[i].begin(), A21._rows[i].end(), res._rows[i + half_size].begin());
            std::move(A22._rows[i].begin(), A22._rows[i].end(), res._rows[i + half_size].begin() + half_size);
        }

        return res;
    }

}

template<typename T>
Matrix<T>::Matrix(const std::size_t &rows, const std::size_t &cols, const T &default_element) :
        _rows(rows, std::vector(cols, default_element)) {}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::vector<T>> elements) :
        _rows{elements} {
    assert(elements.size() != 0 && "No elements provided");
    auto size_tmp = _rows[0].size();
    for (std::size_t i = 1; i < _rows.size(); i++) {
        assert(_rows[i].size() == size_tmp && "Wrong elements sequence provided");
    }
}

template<typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &elements) :
        _rows{elements} {
    assert(elements.size() != 0 && "No elements provided");
    auto size_tmp = _rows[0].size();
    for (std::size_t i = 1; i < _rows.size(); i++) {
        assert(_rows[i].size() == size_tmp && "Wrong elements sequence provided");
    }
}

template<typename T>
std::size_t Matrix<T>::size_rows() const {
    return _rows.size();
}

template<typename T>
std::size_t Matrix<T>::size_cols() const {
    return _rows.empty() ? 0 : _rows[0].size();
}

template<typename T>
T Matrix<T>::operator()(const std::size_t &row, const std::size_t &col) const {
    assert(row < size_rows() && "Out of range");
    assert(col < size_cols() && "Out of range");
    return _rows[row][col];
}

template<typename T>
T &Matrix<T>::operator()(const std::size_t &row, const std::size_t &col) {
    assert(row < size_rows() && "Out of range");
    assert(col < size_cols() && "Out of range");
    return _rows[row][col];
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

    for (int i = 0; i < lhs.size_rows(); i++) {
        for (int j = 0; j < lhs.size_cols(); j++) {
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
void Matrix<T>::print(OStream &os) {
    for (std::size_t i = 0; i < size_rows(); i++) {
        for (std::size_t j = 0; j < size_cols(); j++) {
            os << _rows[i][j] << ' ';
        }
        os << std::endl;
    }
}

template<typename T>
Matrix<T>::~Matrix() {
    for (auto &item : _rows) {
        item.clear();
    }
}

template<typename T>
Matrix<T> multiply(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    assert(lhs.size_cols() == rhs.size_rows() && "Incorrect arguments");

    return detail::defaultMultiplication(std::move(lhs), std::move(rhs));
}

template<typename T>
Matrix<T> multiply(const Matrix<T> &lhs,
                   const Matrix<T> &rhs,
                   MatrixMultiplicationPolicy policy,
                   const std::size_t &size_bound) {
    assert(lhs.size_cols() == rhs.size_rows() && "Incorrect arguments");

    if (policy == MatrixMultiplicationPolicy::Default) {
        return detail::defaultMultiplication(std::move(lhs), std::move(rhs));
    } else {
        return detail::StrassenMultiplication(std::move(lhs), std::move(rhs), policy, size_bound);
    }
}