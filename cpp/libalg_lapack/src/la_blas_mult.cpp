/************************/
/*  la_blas_mult.cpp    */
/*    Version 1.0       */
/*     2023/06/04       */
/************************/

#include <cassert>
#include <complex>
#include <type_traits>
#include <typeinfo>
#include "blas_interface.h"
#include "la_blas_mult.h"
#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"

#define INT_C(x)            static_cast<int>(x)
#define SIZE_T_C(x)         static_cast<size_t>(x)
#define FLOAT_P_R(x)        reinterpret_cast<float*>(x)
#define CONST_FLOAT_P_R(x)  const_cast<float*>(reinterpret_cast<const float*>(x))
#define DOUBLE_P_R(x)       reinterpret_cast<double*>(x)
#define CONST_DOUBLE_P_R(x) const_cast<double*>(reinterpret_cast<const double*>(x))

namespace la
{

    template <typename T> Matrix<T>& MatMultVec(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B)
    {
        assert(A.GetColsNb() == B.GetRowsNb());
        assert(A.GetRowsNb() == res.GetRowsNb());
        assert(B.GetColsNb() == 1);
        assert(res.GetColsNb() == 1);
        char trans = 'N';
        int m = INT_C(A.GetRowsNb()), n = INT_C(A.GetColsNb()), lda = INT_C(A.GetRowsNb()), incx = 1, incy = 1;
        T alpha = 1.0;
        T beta  = 0.0;
        if constexpr (std::is_same_v<T, float>)
        {
            sgemv_(&trans, &m, &n, &alpha, CONST_FLOAT_P_R(A.data().data()), &lda, CONST_FLOAT_P_R(B.data().data()),
                   &incx, &beta, res.data().data(), &incy);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            dgemv_(&trans, &m, &n, &alpha, CONST_DOUBLE_P_R(A.data().data()), &lda, CONST_DOUBLE_P_R(B.data().data()),
                   &incx, &beta, res.data().data(), &incy);
        }
        else if constexpr (std::is_same_v<T, std::complex<float>>)
        {
            cgemv_(&trans, &m, &n, FLOAT_P_R(&alpha), CONST_FLOAT_P_R(A.data().data()), &lda,
                   CONST_FLOAT_P_R(B.data().data()), &incx, FLOAT_P_R(&beta), FLOAT_P_R(res.data().data()), &incy);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>)
        {
            zgemv_(&trans, &m, &n, DOUBLE_P_R(&alpha), CONST_DOUBLE_P_R(A.data().data()), &lda,
                   CONST_DOUBLE_P_R(B.data().data()), &incx, DOUBLE_P_R(&beta), DOUBLE_P_R(res.data().data()), &incy);
        }
        else { throw std::runtime_error("MatMultVec: type not supported"); }
        return res;
    }

    template <typename T> inline Matrix<T>& MatMult(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B)
    {
        assert(res.GetRowsNb() == A.GetRowsNb());
        assert(A.GetColsNb() == B.GetRowsNb());
        assert(A.GetRowsNb() == res.GetRowsNb());
        assert(B.GetColsNb() > 1);
        char transa = 'N', transb = 'N';
        int m = INT_C(A.GetRowsNb()), n = INT_C(B.GetColsNb()), k = INT_C(A.GetColsNb()), lda = INT_C(A.GetRowsNb()),
            ldb = INT_C(B.GetRowsNb()), ldc = INT_C(res.GetRowsNb());
        T alpha = 1.0;
        T beta  = 0.0;
        if constexpr (std::is_same_v<T, float>)
        {
            sgemm_(&transa, &transb, &m, &n, &k, &alpha, CONST_FLOAT_P_R(A.data().data()), &lda,
                   CONST_FLOAT_P_R(B.data().data()), &ldb, &beta, res.data().data(), &ldc);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            dgemm_(&transa, &transb, &m, &n, &k, &alpha, CONST_DOUBLE_P_R(A.data().data()), &lda,
                   CONST_DOUBLE_P_R(B.data().data()), &ldb, &beta, res.data().data(), &ldc);
        }
        else if constexpr (std::is_same_v<T, std::complex<float>>)
        {
            cgemm_(&transa, &transb, &m, &n, &k, FLOAT_P_R(&alpha), CONST_FLOAT_P_R(A.data().data()), &lda,
                   CONST_FLOAT_P_R(B.data().data()), &ldb, FLOAT_P_R(&beta), FLOAT_P_R(res.data().data()), &ldc);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>)
        {
            zgemm_(&transa, &transb, &m, &n, &k, DOUBLE_P_R(&alpha), CONST_DOUBLE_P_R(A.data().data()), &lda,
                   CONST_DOUBLE_P_R(B.data().data()), &ldb, DOUBLE_P_R(&beta), DOUBLE_P_R(res.data().data()), &ldc);
        }
        else { throw std::runtime_error("MatMult: type not supported"); }
        return res;
    }

} // namespace la

#undef CONST_DOUBLE_P_R
#undef DOUBLE_P_R
#undef CONST_FLOAT_P_R
#undef FLOAT_P_R
#undef INT_C
#undef SIZE_T_C

// Explicit template instantiation
template la::Matrix<float>& la::MatMultVec(la::Matrix<float>& res, const la::Matrix<float>& A,
                                           const la::Matrix<float>& B);
template la::Matrix<double>& la::MatMultVec(la::Matrix<double>& res, const la::Matrix<double>& A,
                                            const la::Matrix<double>& B);
template la::Matrix<std::complex<float>>& la::MatMultVec(la::Matrix<std::complex<float>>& res,
                                                         const la::Matrix<std::complex<float>>& A,
                                                         const la::Matrix<std::complex<float>>& B);
template la::Matrix<std::complex<double>>& la::MatMultVec(la::Matrix<std::complex<double>>& res,
                                                          const la::Matrix<std::complex<double>>& A,
                                                          const la::Matrix<std::complex<double>>& B);
template la::Matrix<float>& la::MatMult(la::Matrix<float>& res, const la::Matrix<float>& A, const la::Matrix<float>& B);
template la::Matrix<double>& la::MatMult(la::Matrix<double>& res, const la::Matrix<double>& A,
                                         const la::Matrix<double>& B);
template la::Matrix<std::complex<float>>& la::MatMult(la::Matrix<std::complex<float>>& res,
                                                      const la::Matrix<std::complex<float>>& A,
                                                      const la::Matrix<std::complex<float>>& B);
template la::Matrix<std::complex<double>>& la::MatMult(la::Matrix<std::complex<double>>& res,
                                                       const la::Matrix<std::complex<double>>& A,
                                                       const la::Matrix<std::complex<double>>& B);
