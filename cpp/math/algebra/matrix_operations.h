#ifndef _MATRIX_OPERATIONS_H_83EBD27FF2204642B179FE1A1EB415DE_
#define _MATRIX_OPERATIONS_H_83EBD27FF2204642B179FE1A1EB415DE_

/************************/
/* matrix_operations.h  */
/*    Version 3.0       */
/*     2023/05/23       */
/************************/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include "matrix.h"
#include "tolerance.h"

#define T_C(x) static_cast<T>(x)

namespace la
{

    template <typename T> inline Matrix<T>& MatTranspose(Matrix<T>& res, const Matrix<T>& A)
    {
        assert(res.GetRowsNb() == A.GetColsNb());
        assert(res.GetColsNb() == A.GetRowsNb());
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < A.GetColsNb(); ++j) res(j, i) = A(i, j);
        return res;
    }

    template <typename T> inline Matrix<T> MatTranspose(const Matrix<T>& A)
    {
        Matrix<T> res_{A.GetColsNb(), A.GetRowsNb()};
        return MatTranspose(res_, A);
    }

    template <typename T> inline Matrix<T>& MatConjugate(Matrix<T>& res, const Matrix<T>& A)
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        assert(res.GetColsNb() == A.GetColsNb());

        if constexpr (std::is_same<T, std::complex<double>>::value || std::is_same<T, std::complex<float>>::value)
            for (size_t i = 0; i < A.GetRowsNb(); ++i)
                for (size_t j = 0; j < A.GetColsNb(); ++j) res(i, j) = std::conj(A(i, j));
        else res = A;
        return res;
    }

    template <typename T> inline Matrix<T> MatConjugate(const Matrix<T>& A)
    {
        Matrix<T> res_{A.GetRowsNb(), A.GetColsNb()};
        return MatConjugate(res_, A);
    }

    template <typename T> inline Matrix<T>& MatHermitian(Matrix<T>& res, const Matrix<T>& A)
    {
        assert(res.GetRowsNb() == A.GetColsNb());
        assert(res.GetColsNb() == A.GetRowsNb());
        MatTranspose(res, A);
        if constexpr (std::is_same<T, std::complex<double>>::value || std::is_same<T, std::complex<float>>::value)
            MatConjugate(res, res);
        return res;
    }

    template <typename T> inline Matrix<T> MatHermitian(const Matrix<T>& A)
    {
        Matrix<T> res_{A.GetColsNb(), A.GetRowsNb()};
        return MatHermitian(res_, A);
    }

    template <typename T, typename U> inline Matrix<T>& MatReal(Matrix<T>& res, const Matrix<U>& A)
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        assert(res.GetColsNb() == A.GetColsNb());
        if constexpr (std::is_same<T, std::complex<float>>::value || std::is_same<T, std::complex<double>>::value)
            throw std::runtime_error("MatReal: invalid type For T");

        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < A.GetColsNb(); ++j)
                if constexpr (std::is_same_v<U, double> || std::is_same_v<U, float>) res(i, j) = T_C(A(i, j));
                else if constexpr (std::is_same<U, std::complex<float>>::value ||
                                   std::is_same<U, std::complex<double>>::value)
                    res(i, j) = T_C(std::real(A(i, j)));
                else throw std::runtime_error("MatReal: invalid type For U");
        return res;
    }

    template <typename T, typename U> inline Matrix<T> MatReal(const Matrix<U>& A)
    {
        Matrix<T> res_{A.GetRowsNb(), A.GetColsNb()};
        return MatReal(res_, A);
    }

    template <typename T, typename U> inline Matrix<T>& MatImag(Matrix<T>& res, const Matrix<U>& A)
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        assert(res.GetColsNb() == A.GetColsNb());
        if constexpr (std::is_same<T, std::complex<float>>::value || std::is_same<T, std::complex<double>>::value)
            throw std::runtime_error("MatImag: invalid type For T");
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < A.GetColsNb(); ++j)
                if constexpr (std::is_same_v<U, double> || std::is_same_v<U, float>) res(i, j) = T_C(0);
                else if constexpr (std::is_same<U, std::complex<float>>::value ||
                                   std::is_same<U, std::complex<double>>::value)
                    res(i, j) = T_C(std::imag(A(i, j)));
                else throw std::runtime_error("MatImag: invalid type For U");
        return res;
    }

    template <typename T, typename U> inline Matrix<T> MatImag(const Matrix<U>& A)
    {
        Matrix<T> res_{A.GetRowsNb(), A.GetColsNb()};
        return MatImag(res_, A);
    }

    template <typename T> inline bool MatIsOrthogonal(const Matrix<T>& A, const T tol = DefaultTol<T>())
    {
        assert(A.GetRowsNb() == A.GetColsNb());
        using RealType = typename std::conditional<
            std::is_same<T, std::complex<double>>::value, double,
            typename std::conditional<std::is_same<T, std::complex<float>>::value, float, T>::type>::type;
        RealType tol_;
        if constexpr (std::is_same<T, std::complex<double>>::value || std::is_same<T, std::complex<float>>::value)
            tol_ = tol.real();
        else tol_ = tol;
        bool res = true;
        Matrix<T> aH_{A.GetRowsNb(), A.GetColsNb()}, res_{A.GetRowsNb(), A.GetColsNb()};
        MatHermitian(aH_, A);
        MatMult(res_, A, aH_);
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < A.GetColsNb(); ++j)
                if (i == j)
                {
                    if (std::abs(res_(i, j) - T_C(1)) > tol_) return false;
                }
                else if (std::abs(res_(i, j)) > tol_) return false;
        return res;
    }

    template <typename T>
    inline bool MatIsOrthogonal(const Matrix<T>& A, const Matrix<T>& B, const T& tol = DefaultTol<T>())
    {
        assert(A.GetRowsNb() == B.GetRowsNb());
        assert(A.GetColsNb() == 1);
        assert(B.GetColsNb() == 1);
        return MatDot(A, B) < tol;
    }

    template <typename T>
    inline bool MatIsOrthogonal(const Matrix<std::complex<T>>& A, const Matrix<std::complex<T>>& B,
                                const T tol = DefaultTol<T>())
    {
        assert(A.GetRowsNb() == B.GetRowsNb());
        assert(A.GetColsNb() == 1);
        assert(B.GetColsNb() == 1);
        return MatDot(A, B) < tol;
    }

#ifdef USE_BLAS
    // defined in la_blas_mult.h
    // template <typename T> Matrix<T>& MatMultVec(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B);
#else
    template <typename T> inline Matrix<T>& MatMultVec(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B)
    {
        assert(A.GetColsNb() == B.GetRowsNb());
        assert(A.GetRowsNb() == res.GetRowsNb());
        assert(B.GetColsNb() == 1);
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
        {
            res(i, 0) = T_C(0);
            for (size_t j = 0; j < A.GetColsNb(); ++j) res(i, 0) += A(i, j) * B(j, 0);
        }
        return res;
    }

    template <typename T> inline Matrix<T> MatMultVec(const Matrix<T>& A, const Matrix<T>& B)
    {
        Matrix<T> res_{A.GetRowsNb(), 1};
        return MatMultVec(res_, A, B);
    }

    template <typename T> inline Matrix<T>& MatMult(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B)
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        assert(A.GetColsNb() == B.GetRowsNb());
        assert(A.GetRowsNb() == res.GetRowsNb());
        assert(B.GetColsNb() > 1);
        for (size_t j = 0; j < B.GetColsNb(); ++j)
            for (size_t i = 0; i < A.GetRowsNb(); ++i)
            {
                res(i, j) = T_C(0);
                for (size_t k = 0; k < A.GetColsNb(); ++k) res(i, j) += A(i, k) * B(k, j);
            }

        return res;
    }

    template <typename T> inline Matrix<T> MatMult(const Matrix<T>& A, const Matrix<T>& B)
    {
        Matrix<T> res_{A.GetRowsNb(), B.GetColsNb()};
        return la::MatMult(res_, A, B);
    }

#endif

    template <typename T> inline Matrix<T>& MatHadamard(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B)
    {
        assert(A.GetRowsNb() == res.GetRowsNb());
        assert(A.GetColsNb() == res.GetColsNb());
        assert(A.GetRowsNb() == B.GetRowsNb());
        assert(A.GetColsNb() == B.GetColsNb());
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < A.GetColsNb(); ++j) res(i, j) = A(i, j) * B(i, j);
        return res;
    }

    template <typename T> inline Matrix<T> MatHadamard(const Matrix<T>& A, const Matrix<T>& B)
    {
        Matrix<T> res_{A.GetRowsNb(), A.GetColsNb()};
        return la::MatHadamard(res_, A, B);
    }

    template <typename T> inline Matrix<T>& MatOuter(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B)
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        assert(res.GetColsNb() == B.GetRowsNb());
        assert(A.GetColsNb() == 1);
        assert(B.GetColsNb() == 1);
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < B.GetRowsNb(); ++j) res(i, j) = A(i, 0) * B(j, 0);
        return res;
    }

    template <typename T> inline Matrix<T> MatOuter(const Matrix<T>& A, const Matrix<T>& B)
    {
        Matrix<T> res_{A.GetRowsNb(), B.GetRowsNb()};
        return la::MatOuter(res_, A, B);
    }

    template <typename T> inline T MatDot(const Matrix<T>& A, const Matrix<T>& B)
    {
        assert(A.GetColsNb() == 1);
        assert(B.GetColsNb() == 1);
        T res = T_C(0);
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < A.GetColsNb(); ++j) res += A(i, j) * B(i, j);
        return res;
    }

    template <typename T> inline T MatDot(const Matrix<std::complex<T>>& A, const Matrix<std::complex<T>>& B)
    {
        assert(A.GetColsNb() == 1);
        assert(B.GetColsNb() == 1);
        std::complex<T> res = T_C(0);
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < A.GetColsNb(); ++j) res += std::conj(A(i, j)) * B(i, j);
        return res.real();
    }

    template <typename T>
    Matrix<T>& MatRref(Matrix<T>& res, std::vector<size_t>& vRowsIdx, const size_t& nPivotMax = 0,
                       const T& tol = DefaultTol<T>(), const size_t& nAugCols = 0)
    {
        size_t pivot_{0};
        const bool br_    = !vRowsIdx.empty();
        // Lambda to normalize rows
        auto normalizeRow = [&](const size_t c) {
            // normalize the row with 1 on the pivot
            T v_ = res(c, pivot_);
            for (size_t k = 0; k < res.GetColsNb(); ++k) res(c, k) /= v_;
            for (size_t j = 0; j < res.GetRowsNb(); ++j)
            {
                if (j != c)
                {
                    v_ = res(j, pivot_);
                    for (size_t k = 0; k < res.GetColsNb(); ++k) res(j, k) -= v_ * res(c, k);
                    // ensure the pivot value is zero
                    res(j, pivot_) = T_C(0);
                }
            }
            // if the row below the pivot is zero, swap it with the first non-zero
            bool z1_ = true, z2_ = true;
            if (c == res.GetRowsNb() - 1) return;
            const size_t colsr_ = res.GetColsNb() - nAugCols;
            for (size_t k = 0; k < colsr_; ++k) z1_ = z1_ && (std::abs(res(c + 1, k)) < tol);
            if (z1_)
                for (size_t j = c + 2; j < res.GetRowsNb(); ++j)
                {
                    for (size_t k = 0; k < colsr_; ++k) z2_ = z2_ && (std::abs(res(j, k)) < tol);
                    if (!z2_)
                    {
                        res.SwapRows(c + 1, j);
                        if (br_) std::swap(vRowsIdx[c + 1], vRowsIdx[j]);
                    }
                }
        };

        const size_t pivotMax_ = (nPivotMax == 0) ? std::min({res.GetRowsNb(), res.GetColsNb()})
                                                  : std::min({nPivotMax, res.GetRowsNb(), res.GetColsNb()});
        size_t i{0};
        for (size_t c = 0; c < res.GetRowsNb(); ++c)
        {
            if (pivot_ == pivotMax_) return res;
            i = c;
            while (std::abs(res(i, pivot_)) < tol)
            {
                // set the approximately zero terms to zero.
                res(i, pivot_) = T_C(0);
                ++i;
                if (i >= res.GetRowsNb() - 1)
                {
                    i = c;
                    ++pivot_;
                    if (pivot_ == pivotMax_)
                    {
                        if (res(i, pivot_ - 1) > tol) normalizeRow(c);
                        return res;
                    }
                }
            }
            // Swap the rows
            if (i != c)
            {
                res.SwapRows(i, c);
                if (br_) std::swap(vRowsIdx[i], vRowsIdx[c]);
            }
            normalizeRow(c);
            ++pivot_;
        }
        return res;
    }

    template <typename T>
    Matrix<T>& MatRref(Matrix<T>& res, const size_t& nPivotMax = 0, const T& tol = DefaultTol<T>(),
                       const size_t& nAugCols = 0)
    {
        std::vector<size_t> notused_ = {};
        return MatRref(res, notused_, nPivotMax, tol, nAugCols);
    }

    template <typename T>
    inline Matrix<T>& MatRref(Matrix<T>& res, const Matrix<T>& A, const size_t& nPivotMax = 0,
                              const T& tol = DefaultTol<T>(), const size_t& nAugCols = 0)
    {
        assert(res.size() == A.size());
        res.assign(A.data());
        MatRref(res, nPivotMax, tol, nAugCols);
        return res;
    }

    template <typename T> inline Matrix<T> MatPermutations(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        Matrix<T> res{A.GetRowsNb(), A.GetRowsNb()};
        return MatPermutations(res, A, tol);
    }

    template <typename T> Matrix<T>& MatPermutations(Matrix<T>& res, const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        assert(res.GetRowsNb() == res.GetColsNb());
        // allocate the augmented matrix
        const size_t colsa_ = A.GetColsNb() + A.GetRowsNb();
        Matrix<T> res_{A.GetRowsNb(), colsa_};
        res_.Zeros();
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
        {
            // copy the data in the augmented matrix
            for (size_t j = 0; j < A.GetColsNb(); ++j) res_(i, j) = A(i, j);
            // create the identity matrix
            res_(i, i + A.GetColsNb()) = T_C(1);
        }
        MatRref(res_, 0, tol, A.GetRowsNb());
        // extract the matrix E
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
            for (size_t j = 0; j < A.GetRowsNb(); ++j) res(i, j) = res_(i, j + A.GetColsNb());
        return res;
    }

    template <typename T> Matrix<T>& MatRrefSquare(Matrix<T>& res, const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        // res dimension are ignored
        res = A;
        MatRref(res, A, 0, tol);
        if (A.GetRowsNb() > A.GetColsNb()) return res;
        // make the matrix square
        const size_t n_ = A.GetColsNb() - A.GetRowsNb();
        for (size_t i = 0; i < n_; ++i) res.InsertRow(res.GetRowsNb());
        // ensure that the pivot are on the diagonal
        for (size_t i = 0; i < A.GetRowsNb(); ++i)
        {
            // if there is a zero on the diagonal, look for a non-null element and swap it with the row to
            // be on the diagonal
            if (res(i, i) < tol)
                for (size_t k = i; k < A.GetColsNb(); ++k)
                    if (res(i, k) > tol)
                    {
                        res.SwapRows(i, k);
                        // decrease the index so the row is re-evaluated
                        --i;
                        break;
                    }
        }
        return res;
    }

    template <typename T> inline Matrix<T> MatRrefSquare(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        Matrix<T> res{1, 1};
        return MatRrefSquare(res, A, tol);
    }

    template <typename T> Matrix<T>& MatBasis(Matrix<T>& res, const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        size_t nr_ = 0;
        // if the system is overdimensioned compute the BasisRow of the transpose
        if (A.GetColsNb() < A.GetRowsNb())
        {
            Matrix<T> t_{A.GetColsNb(), A.GetRowsNb()};
            MatTranspose(t_, A);
            return MatBasisRow(res, t_, tol);
        }
        Matrix<T> r_{A.GetRowsNb(), A.GetColsNb()};
        MatRrefSquare(r_, A, tol);
        for (size_t i = 0; i < A.GetColsNb(); ++i)
            if (r_(i, i) > tol)
            {
                if (nr_ != 0) res.InsertCol(res.GetColsNb());
                res.assignCol(nr_, A, i);
                ++nr_;
            }
        // if it is a zero rank return the zero vector
        if (nr_ == 0) res.Zeros();
        return res;
    }

    template <typename T> Matrix<T> MatBasis(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        Matrix<T> res{A.GetRowsNb(), 1};
        return MatBasis(res, A, tol);
    }

    template <typename T> Matrix<T>& MatBasisRow(Matrix<T>& res, const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        assert(res.GetRowsNb() == A.GetColsNb());
        size_t nr_ = 0;
        // if the system is overdimensioned compute the MatBasis of the transpose
        if (A.GetColsNb() < A.GetRowsNb())
        {
            Matrix<T> t_{A.GetColsNb(), A.GetRowsNb()};
            MatTranspose(t_, A);
            return MatBasis(res, t_, tol);
        }
        Matrix<T> r_{A.GetRowsNb(), A.GetColsNb()};
        MatRrefSquare(r_, A, tol);
        for (size_t i = 0; i < A.GetColsNb(); ++i)
            if (r_(i, i) > tol)
            {
                if (nr_ != 0) res.InsertCol(res.GetColsNb());
                res.assignColRow(nr_, r_, i);
                ++nr_;
            }
        if (nr_ == 0) res.Zeros();
        return res;
    }

    template <typename T> Matrix<T> MatBasisRow(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        Matrix<T> res{A.GetColsNb(), 1};
        return MatBasisRow(res, A, tol);
    }

    template <typename T> Matrix<T>& MatNullspace(Matrix<T>& res, const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        assert(res.GetRowsNb() == A.GetColsNb());
        size_t ns_ = 0;
        // if the system is overdimensioned compute the MatLeftNullspace of the transpose
        if (A.GetColsNb() < A.GetRowsNb())
        {
            Matrix<T> t_{A.GetColsNb(), A.GetRowsNb()};
            MatTranspose(t_, A);
            return MatLeftNullspace(res, t_, tol);
        }
        Matrix<T> r_{A.GetRowsNb(), A.GetColsNb()};
        MatRrefSquare(r_, A, tol);
        for (size_t i = 0; i < A.GetColsNb(); ++i)
            if (r_(i, i) < tol)
            {
                // the zero for the free column need to be replace with -1
                r_(i, i) = T_C(-1);
                if (ns_ != 0) res.InsertCol(res.GetColsNb());
                // the correspondent column is a vector of the null space
                res.assignCol(ns_, r_, i);
                ++ns_;
            }
        // if it is a full rank return the zero vector
        if (ns_ == 0) res.Zeros();
        return res;
    }

    template <typename T> Matrix<T> MatNullspace(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        Matrix<T> res{A.GetColsNb(), 1};
        return MatNullspace(res, A, tol);
    }

    template <typename T>
    Matrix<T>& MatLeftNullspace(Matrix<T>& res, const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        size_t ns_ = 0;
        // if the system is overdimensioned compute the MatNullspace of the transpose
        if (A.GetColsNb() < A.GetRowsNb())
        {
            Matrix<T> t_{A.GetColsNb(), A.GetRowsNb()};
            MatTranspose(t_, A);
            return MatNullspace(res, t_, tol);
        }
        Matrix<T> r_{A.GetRowsNb(), A.GetColsNb()}, pm_{A.GetRowsNb(), A.GetRowsNb()};
        // compute the rref
        r_ = A;
        MatRref(r_, 0, tol);
        // get the Gauss Jordan Matrix
        MatPermutations(pm_, A);
        for (size_t i = 0; i < r_.GetRowsNb(); ++i)
        {
            bool e_ = true;
            for (size_t j = 0; j < r_.GetColsNb(); ++j)
                if (r_(i, j) > tol)
                {
                    // row is not empty, process the next
                    e_ = false;
                    break;
                }
            if (e_)
            {
                if (ns_ != 0) res.InsertCol(res.GetColsNb());
                // copy the row from the permutation matrix in the result
                res.assignColRow(ns_, pm_, i);
                ns_++;
            }
        }
        // if it is a full rank return the zero vector
        if (ns_ == 0) res.Zeros();
        return res;
    }

    template <typename T> Matrix<T> MatLeftNullspace(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        Matrix<T> res{A.GetRowsNb(), 1};
        return MatLeftNullspace(res, A, tol);
    }

#ifndef USE_LAPACK
    template <typename T> size_t MatRank(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        size_t res_ = 0;
        Matrix<T> r_{A.GetRowsNb(), A.GetColsNb()};
        MatRrefSquare(r_, A, tol);
        for (size_t i = 0; i < r_.GetColsNb(); ++i)
            if (r_(i, i) > tol) ++res_;
        return res_;
    }
#endif

    template <typename T>
    std::vector<size_t>& MatColsNb(std::vector<size_t>& res, const Matrix<T>& A, const T& tol, const bool& pivot)
    {
        Matrix<T> r_{A.GetRowsNb(), A.GetColsNb()};
        MatRrefSquare(r_, A, tol);
        for (size_t i = 0; i < r_.GetColsNb(); ++i)
            if (pivot && r_(i, i) > tol) res.push_back(i);
            else if (!pivot && r_(i, i) < tol) res.push_back(i);
        return res;
    }

    template <typename T> std::vector<size_t> MatPivotColsNb(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        std::vector<size_t> res_;
        return MatColsNb(res_, A, tol, true);
    }

    template <typename T> std::vector<size_t> MatFreeColsNb(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        std::vector<size_t> res_;
        return MatColsNb(res_, A, tol, false);
    }

    template <typename T>
    std::vector<size_t> MatRowsNb(std::vector<size_t>& res, const Matrix<T>& A, const T& tol, const bool pivot)
    {
        Matrix<T> r_(A);
        std::vector<size_t> vRowsIdx_(A.GetRowsNb());
        std::iota(vRowsIdx_.begin(), vRowsIdx_.end(), 0);
        // compute the rref
        MatRref(r_, vRowsIdx_, 0, tol);

        for (size_t i = 0; i < r_.GetRowsNb(); ++i)
        {
            bool e_ = true;
            for (size_t j = 0; j < r_.GetColsNb(); ++j)
                if (r_(i, j) > tol)
                {
                    // row is not empty, process the next
                    e_ = false;
                    break;
                }
            // check if is not empty and is pivot
            if (!e_ && pivot) res.push_back(vRowsIdx_[i]);
            // check if is empty and is free row
            else if (e_ && !pivot) res.push_back(vRowsIdx_[i]);
        }
        return res;
    }

    template <typename T> std::vector<size_t> MatPivotRowsNb(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        std::vector<size_t> res_;
        return MatRowsNb(res_, A, tol, true);
    }

    template <typename T> std::vector<size_t> MatFreeRowsNb(const Matrix<T>& A, const T& tol = DefaultTol<T>())
    {
        std::vector<size_t> res_;
        return MatRowsNb(res_, A, tol, false);
    }

    // compute AT * A
    template <typename T> Matrix<T>& MatAtA(Matrix<T>& res, const Matrix<T>& A)
    {
        assert(res.GetRowsNb() == A.GetColsNb());
        assert(res.GetColsNb() == A.GetColsNb());
        for (size_t i = 0; i < A.GetColsNb(); ++i)
            for (size_t j = 0; j < A.GetColsNb(); ++j)
            {
                T sum_ = 0;
                for (size_t k = 0; k < A.GetRowsNb(); ++k) sum_ += A(k, i) * A(k, j);
                res(i, j) = sum_;
            }
        return res;
    }

    template <typename T> Matrix<T> MatAtA(const Matrix<T>& A)
    {
        Matrix<T> res{A.GetColsNb(), A.GetColsNb()};
        return MatAtA(res, A);
    }

} // namespace la

#undef T_C

#endif
