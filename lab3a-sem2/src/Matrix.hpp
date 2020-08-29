#pragma once

#include <type_traits>
#include <vector>

/**
 * @brief a class for storing numerical matrix
 * @tparam T arithmetic type of elements stored in matrix
 */
template<typename T>
class Matrix {
    static_assert(std::is_arithmetic_v<T>);

public:
    Matrix() = delete;

    /**
     * @brief constructor
     * @param size_r number of rows
     * @param size_c number of columns
     * @param default_element a value to fill matrix with
     */
    Matrix(const std::size_t &rows, const std::size_t &cols, const T &default_element = 0);

    Matrix(std::initializer_list<std::vector<T>> elements);

    /**
     * @brief gives access to an element value
     * @param row element's row index
     * @param col element's column index
     * @return value stored in corresponding place
     */
    T operator()(const std::size_t &row, const std::size_t &col) const;

    /**
     * @brief gives access to an element
     * @param row element's row index
     * @param col element's column index
     * @return a reference to value stored in corresponding place
     */
    T &operator()(const std::size_t &row, const std::size_t &col);

    /**
     * @return number of rows in matrix
     */
    std::size_t size_rows() const;

    /**
     * @return nu,ber of columns in matrix
     */
    std::size_t size_cols() const;

    /**
     * @brief prints matrix to output stream
     * @tparam OStream type of stream tp be used
     * @param os a stream reference
     */
    template<typename OStream>
    void print(OStream& os);

private:
    std::vector<std::vector<T>> _rows_data;

};

template<typename T>
Matrix<T> operator+(const Matrix<T> &lhs, const Matrix<T> &rhs);

template<typename T>
Matrix<T> operator-(const Matrix<T> &lhs, const Matrix<T> &rhs);

template<typename T>
bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs);

template<typename T>
bool operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs);

#include "Matrix.hxx"