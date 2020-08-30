#pragma once

#include <type_traits>
#include <vector>

template<typename T>
class Matrix;

enum class MatrixMultiplicationPolicy {
    Default, Strassen, StrassenParallel
};

namespace detail {
    template<typename TT>
    Matrix<TT> defaultMultiplication(const Matrix<TT> &lhs, const Matrix<TT> &rhs);

    template<typename TT>
    Matrix<TT> StrassenMultiplication(const Matrix<TT> &lhs,
                                      const Matrix<TT> &rhs,
                                      MatrixMultiplicationPolicy policy,
                                      const std::size_t &size_bound
    );

    template<typename TT>
    Matrix<TT> StrassenMultiplicationStep(const Matrix<TT> &lhs,
                                          const Matrix<TT> &rhs,
                                          const std::size_t &size,
                                          const std::size_t &size_bound
    );

    template<typename TT>
    Matrix<TT> StrassenMultiplicationStepParallel(const Matrix<TT> &lhs,
                                                  const Matrix<TT> &rhs,
                                                  const std::size_t &size,
                                                  const std::size_t &size_bound
    );
}

/**
 * @brief a class for storing numerical matrix
 * @tparam T arithmetic type of elements stored in matrix
 */
template<typename T>
class Matrix {
    static_assert(std::is_arithmetic_v<T>);

public:
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
    [[nodiscard]] std::size_t size_rows() const;

    /**
     * @return number of columns in matrix
     */
    [[nodiscard]] std::size_t size_cols() const;

    /**
     * @brief prints matrix to output stream
     * @tparam OStream type of stream tp be used
     * @param os a stream reference
     */
    template<typename OStream>
    void print(OStream &os);

    template<typename TT>
    friend Matrix<TT> detail::defaultMultiplication(const Matrix<TT> &lhs, const Matrix<TT> &rhs);

    template<typename TT>
    friend Matrix<TT> detail::StrassenMultiplication(const Matrix<TT> &lhs,
                                                     const Matrix<TT> &rhs,
                                                     MatrixMultiplicationPolicy policy,
                                                     const std::size_t &size_bound
    );

    template<typename TT>
    friend Matrix<TT> detail::StrassenMultiplicationStep(const Matrix<TT> &lhs,
                                                         const Matrix<TT> &rhs,
                                                         const std::size_t &size,
                                                         const std::size_t &size_bound
    );

    template<typename TT>
    friend Matrix<TT> detail::StrassenMultiplicationStepParallel(const Matrix<TT> &lhs,
                                                                 const Matrix<TT> &rhs,
                                                                 const std::size_t &size,
                                                                 const std::size_t &size_bound
    );


    ~Matrix();
private:
    std::vector<std::vector<T>> _rows;

};

template<typename T>
Matrix<T> operator+(const Matrix<T> &lhs, const Matrix<T> &rhs);

template<typename T>
Matrix<T> operator-(const Matrix<T> &lhs, const Matrix<T> &rhs);

template<typename T>
bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs);

template<typename T>
bool operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs);

/**
 * @brief matrix multiplication
 * @tparam T arithmetic type of elements stored in matrix
 * @param lhs left multiplication argument
 * @param rhs right multiplication argument
 * @return result os multiplication
 */
template<typename T>
Matrix<T> multiply(const Matrix<T> &lhs,
                   const Matrix<T> &rhs);

constexpr std::size_t SIZE_BOUND = 64;

/**
 * @brief matrix multiplication
 * @tparam T arithmetic type of elements stored in matrix
 * @param lhs left multiplication argument
 * @param rhs right multiplication argument
 * @param policy multiplication policy
 * @return result os multiplication
 */
template<typename T>
Matrix<T> multiply(const Matrix<T> &lhs,
                   const Matrix<T> &rhs,
                   MatrixMultiplicationPolicy policy,
                   const std::size_t &size_bound = SIZE_BOUND
);


#include "Matrix.hxx"