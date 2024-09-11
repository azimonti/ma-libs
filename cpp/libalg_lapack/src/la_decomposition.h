#ifndef _LA_DECOMPOSITION_H_71BD600FBE0F4DD6B47509B9D87C8C2F_
#define _LA_DECOMPOSITION_H_71BD600FBE0F4DD6B47509B9D87C8C2F_

/**********************/
/* la_decomposition.h */
/*      Version 3.0   */
/*     2023/05/23     */
/**********************/

#ifdef USE_LAPACK
#error "USE_LAPACK is defined use la_lapack_.. routines"
#endif

#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <iostream>
#include <numeric>
#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"

#define T_C(x) static_cast<T>(x)

namespace la
{

    // compute Gram-Schmidt orthogonalization
    template <typename T> Matrix<T>& MatGramSchmidt(Matrix<T>& res, const Matrix<T>& A, const bool& decreasing = false)
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        assert(res.GetColsNb() == A.GetColsNb());

        Matrix<T> tmp{A.GetRowsNb(), A.GetColsNb()};
        // copy the matrix A and reorder based on the  norm of column from the bigger
        // to the smaller
        std::vector<size_t> idx(A.GetColsNb());
        std::iota(idx.begin(), idx.end(), 0);
        if (decreasing)
        {
            std::sort(idx.begin(), idx.end(), [&A](size_t i1, size_t i2) {
                T sum1 = 0;
                T sum2 = 0;
                for (size_t i = 0; i < A.GetRowsNb(); ++i)
                {
                    sum1 += A(i, i1) * A(i, i1);
                    sum2 += A(i, i2) * A(i, i2);
                }
                return sum1 > sum2;
            });
        }
        for (size_t i = 0; i < A.GetColsNb(); ++i)
            for (size_t j = 0; j < A.GetRowsNb(); ++j) tmp(j, i) = A(j, idx[i]);

        for (size_t i = 0; i < A.GetColsNb(); ++i)
        {
            // copy the column
            for (size_t j = 0; j < A.GetRowsNb(); ++j) res(j, i) = tmp(j, i);
            // subtract the projection of the previous columns
            for (size_t j = 0; j < i; ++j)
            {
                T sum_ = 0;
                for (size_t k = 0; k < A.GetRowsNb(); ++k) sum_ += tmp(k, i) * res(k, j);
                for (size_t k = 0; k < A.GetRowsNb(); ++k) res(k, i) -= sum_ * res(k, j);
            }
            // normalize the column
            T sum_ = 0;
            for (size_t k = 0; k < A.GetRowsNb(); ++k) sum_ += res(k, i) * res(k, i);
            sum_ = sqrt(sum_);
            if (sum_ != T_C(0))
                for (size_t k = 0; k < A.GetRowsNb(); ++k) res(k, i) /= sum_;
        }
        return res;
    }

    template <typename T> Matrix<T> MatGramSchmidt(const Matrix<T>& A)
    {
        Matrix<T> res{A.GetRowsNb(), A.GetColsNb()};
        return MatGramSchmidt(res, A);
    }

    // compute the QR decomposition
    // a = q * r
    template <typename T>
    Matrix<T>& MatQR(Matrix<T>& Q, Matrix<T>& R, const Matrix<T>& A, const bool& decreasing = false)
    {
        assert(Q.GetRowsNb() == A.GetRowsNb());
        assert(Q.GetColsNb() == A.GetRowsNb());
        assert(R.GetRowsNb() == A.GetColsNb());
        assert(R.GetColsNb() == A.GetColsNb());
        // compute the Gram-Schmidt orthogonalization
        MatGramSchmidt(Q, A, decreasing);
        // q is order by the norm of the column from the bigger to the smaller
        // compute the norm of the columns and order it
        std::vector<size_t> idx(A.GetColsNb());
        std::iota(idx.begin(), idx.end(), 0);
        if (decreasing)
        {
            std::sort(idx.begin(), idx.end(), [&A](size_t i1, size_t i2) {
                T sum1 = 0;
                T sum2 = 0;
                for (size_t i = 0; i < A.GetRowsNb(); ++i)
                {
                    sum1 += A(i, i1) * A(i, i1);
                    sum2 += A(i, i2) * A(i, i2);
                }
                return sum1 > sum2;
            });
        }
        // compute the R matrix using the ordered columns (if decreasing is true) or the original columns
        for (size_t i = 0; i < A.GetColsNb(); ++i)
            for (size_t j = 0; j <= i; ++j)
            {
                T sum_ = 0;
                for (size_t k = 0; k < A.GetRowsNb(); ++k) sum_ += A(k, idx[i]) * Q(k, j);
                R(j, i) = sum_;
            }
        return Q;
    }

    template <typename T> Matrix<T> MatQR(Matrix<T>& R, const Matrix<T>& A)
    {
        Matrix<T> Q{A.GetRowsNb(), A.GetColsNb()};
        return MatQR(Q, R, A);
    }

    template <typename T> Matrix<T> MatQR(const Matrix<T>& A)
    {
        Matrix<T> Q{A.GetRowsNb(), A.GetColsNb()};
        Matrix<T> R{A.GetColsNb(), A.GetColsNb()};
        return MatQR(Q, R, A);
    }

    // compute the determinant of a matrix
    template <typename T> T MatDet(const Matrix<T>& A)
    {
        assert(A.GetRowsNb() == A.GetColsNb());
        if (A.GetRowsNb() == 1) return A(0, 0);
        if (A.GetRowsNb() == 2) return A(0, 0) * A(1, 1) - A(0, 1) * A(1, 0);
        T res = 0;
        for (size_t i = 0; i < A.GetColsNb(); ++i)
        {
            Matrix<T> sub{A.GetRowsNb() - 1, A.GetColsNb() - 1};
            for (size_t j = 1; j < A.GetRowsNb(); ++j)
                for (size_t k = 0; k < A.GetColsNb(); ++k)
                    if (k < i) sub(j - 1, k) = A(j, k);
                    else if (k > i) sub(j - 1, k - 1) = A(j, k);
            res += A(0, i) * MatDet(sub) * (i % 2 == 0 ? 1 : -1);
        }
        return res;
    }

    // compute the determinant of a matrix using Gaussian elimination
    template <typename T> T MatDetGauss(const Matrix<T>& A)
    {
        assert(A.GetRowsNb() == A.GetColsNb());
        Matrix<T> b{A};
        T res = 1;
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
        {
            // find the pivot
            size_t pivot = i;
            for (size_t j = i + 1; j < A.GetRowsNb(); ++j)
                if (fabs(b(j, i)) > fabs(b(pivot, i))) pivot = j;
            // swap the rows
            if (pivot != i)
            {
                for (size_t j = 0; j < A.GetColsNb(); ++j) std::swap(b(i, j), b(pivot, j));
                res = -res;
            }
            // eliminate the column
            for (size_t j = i + 1; j < A.GetRowsNb(); ++j)
            {
                T f = b(j, i) / b(i, i);
                for (size_t k = i; k < A.GetColsNb(); ++k) b(j, k) -= f * b(i, k);
            }
            res *= b(i, i);
        }
        return res;
    }

    template <typename T> Matrix<T>& MatEigen(Matrix<T>& res, const Matrix<T>& A, const bool& decreasing = false)
    {
        assert(A.GetRowsNb() == A.GetColsNb());
        res = A;
        // qr decomposition
        Matrix<T> q{A.GetColsNb(), A.GetColsNb()};
        Matrix<T> r{A.GetColsNb(), A.GetColsNb()};
        // compute the eigenvalues, the number of iterations is arbitrary
        for (size_t i = 0; i < 500; ++i)
        {
            MatQR(q, r, res, decreasing);
            MatMult(res, r, q);
        }
        return res;
    }

    template <typename T>
    std::vector<T>& MatEigen(std::vector<T>& res, const Matrix<T>& A, const bool& decreasing = false)
    {
        assert(A.GetRowsNb() == A.GetColsNb());
        res.resize(A.GetColsNb());
        Matrix<T> eigen{A};
        MatEigen(eigen, A, decreasing);
        for (size_t i = 0; i < A.GetColsNb(); ++i) res[i] = eigen(i, i);
        return res;
    }

    template <typename T> std::vector<T> MatEigen(const Matrix<T>& A, bool decreasing)
    {
        std::vector<T> res;
        return MatEigen(res, A, decreasing);
    }

    template <typename T>
    Matrix<std::complex<T>>& MatEigen(Matrix<std::complex<T>>& res, const Matrix<T>& A, const bool& decreasing)
    {
        assert(A.GetRowsNb() == A.GetColsNb());
        Matrix<T> res_ = A;
        // qr decomposition
        Matrix<T> q{A.GetColsNb(), A.GetColsNb()};
        Matrix<T> r{A.GetColsNb(), A.GetColsNb()};
        // compute the Schur decomposition, the number of iterations is arbitrary
        for (size_t i = 0; i < 500; ++i)
        {
            MatQR(q, r, res_, decreasing);
            MatMult(res_, r, q);
        }
        // compute the eigenvalues
        res = Matrix<std::complex<T>>(A.GetColsNb(), 1);
        // check on each 2x2 block if the eigenvalues are real or complex
        for (size_t i = 0; i < A.GetColsNb(); ++i)
        {
            if (i < A.GetColsNb() - 1 && fabs(res_(i + 1, i)) > 1e-10)
            {
                // the eigenvalues are complex
                std::complex<T> a     = res_(i, i);
                std::complex<T> b     = res_(i, i + 1);
                std::complex<T> c     = res_(i + 1, i);
                std::complex<T> d     = res_(i + 1, i + 1);
                std::complex<T> delta = sqrt((a + d) * (a + d) - T_C(4) * (a * d - b * c));
                res(i, 0)             = (a + d + delta) / T_C(2);
                res(i + 1, 0)         = (a + d - delta) / T_C(2);
                ++i;
            }
            else
            {
                // the eigenvalues are real
                res(i, 0) = res_(i, i);
            }
        }
        return res;
    }

    template <typename T>
    std::vector<std::complex<T>>& MatEigen(std::vector<std::complex<T>>& res, const Matrix<T>& A,
                                           const bool& decreasing = false)
    {
        assert(A.GetRowsNb() == A.GetColsNb());
        res.resize(A.GetColsNb());
        Matrix<std::complex<T>> eigen(A.GetColsNb());
        MatEigen(eigen, A, decreasing);
        for (size_t i = 0; i < A.GetColsNb(); ++i) res[i] = eigen(i, 0);
        return res;
    }
} // namespace la

#undef T_C

#endif
