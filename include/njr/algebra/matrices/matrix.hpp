#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <njr/algebra/core/core.hpp>

namespace njr::algebra {

template<RingElement T, std::size_t Rows, std::size_t Columns>
class MatrixSpace;

template<RingElement T, std::size_t Rows, std::size_t Columns>
class Matrix : public AdditiveOperators<Matrix<T, Rows, Columns>> {
    static_assert(Rows > 0 && Columns > 0);
    static_assert(Rows <= std::numeric_limits<std::size_t>::max() / Columns);
    static constexpr std::size_t extent = Rows * Columns;

    template<std::size_t... I>
    static std::array<T, extent> zero_array(std::index_sequence<I...>) {
        return {((void)I, T::zero())...};
    }

    std::array<T, extent> values_ = zero_array(std::make_index_sequence<extent>{});

    static void check_index(std::size_t row, std::size_t column) {
        if (row >= Rows || column >= Columns) {
            throw std::out_of_range("Matrix index is out of range");
        }
    }

public:
    using coefficient_type = T;
    using parent_type = MatrixSpace<T, Rows, Columns>;
    using algebra = std::conditional_t<Rows == Columns,
        std::conditional_t<Rows == 1 && CommutativeRingElement<T>,
            CommutativeRingDescriptor, RingDescriptor>, AdditiveGroupDescriptor>;
    static constexpr std::size_t rows = Rows;
    static constexpr std::size_t columns = Columns;

    Matrix() = default;

    Matrix(std::initializer_list<std::initializer_list<T>> values) {
        if (values.size() != Rows) {
            throw std::invalid_argument("Incorrect number of matrix rows");
        }
        std::size_t row = 0;
        for (const auto& input_row : values) {
            if (input_row.size() != Columns) {
                throw std::invalid_argument("Incorrect number of matrix columns");
            }
            std::size_t column = 0;
            for (const auto& value : input_row) {
                values_[row * Columns + column++] = value;
            }
            ++row;
        }
    }

    static Matrix zero() {
        return Matrix{};
    }

    static Matrix one() requires (Rows == Columns) {
        Matrix result;
        for (std::size_t i = 0; i < Rows; ++i) {
            result(i, i) = T::one();
        }
        return result;
    }

    T& operator()(std::size_t row, std::size_t column) {
        check_index(row, column);
        return values_[row * Columns + column];
    }

    const T& operator()(std::size_t row, std::size_t column) const {
        check_index(row, column);
        return values_[row * Columns + column];
    }

    T& at(std::size_t row, std::size_t column) {
        return (*this)(row, column);
    }

    const T& at(std::size_t row, std::size_t column) const {
        return (*this)(row, column);
    }

    bool is_zero() const {
        for (const auto& value : values_) {
            if (!value.is_zero()) {
                return false;
            }
        }
        return true;
    }

    bool is_one() const requires (Rows == Columns) {
        for (std::size_t i = 0; i < Rows; ++i) {
            for (std::size_t j = 0; j < Columns; ++j) {
                if (i == j) {
                    if (!(*this)(i, j).is_one()) {
                        return false;
                    }
                } else if (!(*this)(i, j).is_zero()) {
                    return false;
                }
            }
        }
        return true;
    }

    Matrix add(const Matrix& other) const {
        Matrix result;
        for (std::size_t i = 0; i < extent; ++i) {
            result.values_[i] = values_[i].add(other.values_[i]);
        }
        return result;
    }

    Matrix negate() const {
        Matrix result;
        for (std::size_t i = 0; i < extent; ++i) {
            result.values_[i] = values_[i].negate();
        }
        return result;
    }

    template<std::size_t OtherColumns>
    Matrix<T, Rows, OtherColumns> multiply(const Matrix<T, Columns, OtherColumns>& other) const {
        Matrix<T, Rows, OtherColumns> result;
        for (std::size_t i = 0; i < Rows; ++i) {
            for (std::size_t j = 0; j < OtherColumns; ++j) {
                T sum = T::zero();
                for (std::size_t k = 0; k < Columns; ++k) {
                    const T product = (*this)(i, k).multiply(other(k, j));
                    sum = sum.add(product);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    template<std::size_t OtherColumns>
    Matrix<T, Rows, OtherColumns> operator*(const Matrix<T, Columns, OtherColumns>& other) const {
        return multiply(other);
    }

    Matrix& operator*=(const Matrix& other) requires (Rows == Columns) {
        *this = multiply(other);
        return *this;
    }

    Matrix scale_left(const T& scalar) const {
        Matrix result;
        for (std::size_t i = 0; i < extent; ++i) {
            result.values_[i] = scalar.multiply(values_[i]);
        }
        return result;
    }

    Matrix scale_right(const T& scalar) const {
        Matrix result;
        for (std::size_t i = 0; i < extent; ++i) {
            result.values_[i] = values_[i].multiply(scalar);
        }
        return result;
    }

    Matrix<T, Columns, Rows> transpose() const {
        Matrix<T, Columns, Rows> result;
        for (std::size_t i = 0; i < Rows; ++i) {
            for (std::size_t j = 0; j < Columns; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    void swap_rows(std::size_t first, std::size_t second) {
        if (first >= Rows || second >= Rows) {
            throw std::out_of_range("Matrix row index is out of range");
        }
        for (std::size_t j = 0; j < Columns; ++j) {
            std::swap((*this)(first, j), (*this)(second, j));
        }
    }

    const parent_type& parent() const {
        static const parent_type instance;
        return instance;
    }

    bool operator==(const Matrix& other) const {
        return values_ == other.values_;
    }
};

template<RingElement T, std::size_t Rows, std::size_t Columns>
class MatrixSpace : public AdditiveGroupStructure<Matrix<T, Rows, Columns>> {
public:
    using Element = Matrix<T, Rows, Columns>;
};

template<RingElement T, std::size_t N>
class MatrixSpace<T, N, N> : public RingStructureFor<Matrix<T, N, N>> {
public:
    using Element = Matrix<T, N, N>;
};

template<RingElement T, std::size_t N>
using MatrixRing = MatrixSpace<T, N, N>;

}
