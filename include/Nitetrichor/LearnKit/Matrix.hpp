/*!
* Created by Molybdenum on 9/23/21.
*
*/
#ifndef NITETRICHOR_MATRIX_HPP
#define NITETRICHOR_MATRIX_HPP

#include <iostream>
namespace NC {


template<typename Matrix>
struct Matrix_trait {
    typedef typename Matrix::column_size column_size;
    typedef typename Matrix::row_size row_size;
};

template<int Row = 4, int Column = 4, typename T = int>
struct Matrix {

    typedef std::integral_constant<T, Row> column_size;
    typedef std::integral_constant<T, Column> row_size;

    T matrix[Row][Column];

    constexpr Matrix(std::initializer_list<T> list) noexcept {
        auto iter = list.begin();
        for (int j = 0; j < Row; ++j) {
            for (int k = 0; k < Column; ++k) {
                matrix[j][k] = *(iter++);
            }
        }
    }

    constexpr Matrix() noexcept : matrix{} {}

    constexpr static Matrix Identity() noexcept {
        Matrix ret;
        T *m = ret.matrix[0];
        for (int i = 0; i < std::min(Row, Column); ++i) {
            *m = 1;
            m += Column + 1;
        }
        return ret;
    }

    inline const T* operator[] (size_t index) const noexcept{ return matrix[index]; }

    inline T* operator[] (size_t index) noexcept { return matrix[index]; }

    template<typename U>
    Matrix operator * (U num) noexcept {
        Matrix ret = *this;
        for (auto& row : ret.matrix) {
            for (auto &val : row) {
                val *= num;
            }
        }
        return ret;
    }

    template<typename U>
    Matrix operator % (U num) noexcept {
        Matrix ret = *this;
        for (auto& row : ret.matrix) {
            for (auto &val : row) {
                val %= num;
            }
        }
        return ret;
    }

    template<int R, int C, typename U>
    [[nodiscard]] Matrix<Row, C, T> mulMod (const Matrix<R, C, T> &m, U mod) const noexcept {
        Matrix<Row, C, T> ret;
        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < C; ++j) {
                for (int k = 0; k < Column; ++k) {
                    ret[i][j] += (matrix[i][k] % mod) * (m[k][j] % mod);
                    ret[i][j] %= mod;
                }

            }
        }
        return ret;
    }

    template<int R, int C>
    Matrix<Row, C, T> operator * (const Matrix<R, C, T> &m) const noexcept {
        Matrix<Row, C, T> ret;
        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < C; ++j) {
                for (int k = 0; k < Column; ++k) {
                    ret[i][j] += matrix[i][k] * m[k][j];
                }
            }
        }
        return ret;
    }

};

template<typename U, int R, int C>
std::ostream &operator << (std::ostream &os, const Matrix<R, C, U> &outM) {
    auto m = outM.matrix;
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            if (j == 0) {
                os << m[i][j];
            } else {
                os << ' ' << m[i][j];
            }
        }
        os << std::endl;
    }
    return os;
}

template<int R, int C, typename T, typename U>
Matrix<R, C, T> Pow(Matrix<R, C, T> x, U y) {
    // x^y, see y as binary.
    // e.g. y = 1010, ret = x^1000 * x^0010, each time x = x^2 y >>= 1, when (y&1) == 1, ret *= x.
    // x = a^0001, and x^2 = a^0010, x^3 = a^0100, ...
    auto ret = Matrix<R, C, T>::Identity();
    while (y) {
        if (y & 1) {
            ret = ret * x;
        }
        x = x * x;
        y >>= 1;
    }
    return ret;
}


template<int R, int C, typename T, typename U, typename V>
Matrix<R, C, T> PowMod(Matrix<R, C, T> x, U y, V mod) {
    // x^y, see y as binary.
    // e.g. y = 1010, ret = x^1000 * x^0010, each time x = x^2 y >>= 1, when (y&1) == 1, ret *= x.
    // x = a^0001, and x^2 = a^0010, x^3 = a^0100, ...
    auto ret = Matrix<R, C, T>::Identity();
    while (y) {
        if (y & 1) {
            ret = ret.mulMod(x, mod);
        }
        x = x.mulMod(x, mod);
        y >>= 1;
    }
    return ret;
}





}

#endif //NITETRICHOR_MATRIX_HPP
