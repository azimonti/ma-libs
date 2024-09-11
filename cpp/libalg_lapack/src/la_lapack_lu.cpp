/************************/
/*  la_lapack_lu.cpp    */
/*    Version 1.0       */
/*     2023/06/13       */
/************************/

#include <cassert>
#include <complex>
#include <type_traits>
#include <typeinfo>
#include "la_blas_mult.h"
#include "la_lapack_lu.h"
#include "lapack_interface.h"

#define T_C(x)        static_cast<T>(x)
#define INT_C(x)      static_cast<int>(x)
#define SIZE_T_C(x)   static_cast<size_t>(x)
#define FLOAT_P_R(x)  reinterpret_cast<float*>(x)
#define DOUBLE_P_R(x) reinterpret_cast<double*>(x)

namespace la
{
    template <typename T>
    Matrix<T>& MatLU(Matrix<T>& L, Matrix<T>& U, Matrix<T>& P, const Matrix<T>& A, const int& flags)
    {
        assert(P.GetColsNb() == L.GetRowsNb());
        assert(P.GetColsNb() == P.GetRowsNb());
        assert(L.GetColsNb() == U.GetRowsNb());
        (void)flags;
        Matrix<T> Atmp = A;
        int m = INT_C(A.GetRowsNb()), n = INT_C(A.GetColsNb()), lda = m, ipv = std::min(m, n), info = 0;
        std::vector<int> ipiv(SIZE_T_C(ipv));
        if constexpr (std::is_same_v<T, float>) sgetrf_(&m, &n, Atmp.data().data(), &lda, ipiv.data(), &info);
        else if constexpr (std::is_same_v<T, double>) dgetrf_(&m, &n, Atmp.data().data(), &lda, ipiv.data(), &info);
        else if constexpr (std::is_same_v<T, std::complex<float>>)
            cgetrf_(&m, &n, FLOAT_P_R(Atmp.data().data()), &lda, ipiv.data(), &info);
        else if constexpr (std::is_same_v<T, std::complex<double>>)
            zgetrf_(&m, &n, DOUBLE_P_R(Atmp.data().data()), &lda, ipiv.data(), &info);
        else throw std::runtime_error("MatLU: unsupported type");
        if (info < 0) throw std::runtime_error("MatLU: illegal value");
        else if (info > 0) throw std::runtime_error("MatLU: singular matrix");
        // copy L (lower triangular or lower trapezoidal matrix)
        for (size_t i = 0; i < SIZE_T_C(L.GetRowsNb()); ++i)
        {
            for (size_t j = 0; j < SIZE_T_C(L.GetColsNb()); ++j)
            {
                if (i > j) L(i, j) = Atmp(i, j);
                else L(i, j) = i == j ? T_C(1) : T_C(0);
            }
        }
        // copy U (upper triangular or upper trapezoidal matrix)
        for (size_t i = 0; i < SIZE_T_C(U.GetRowsNb()); ++i)
        {
            for (size_t j = 0; j < SIZE_T_C(U.GetColsNb()); ++j)
            {
                if (i <= j) U(i, j) = Atmp(i, j);
                else U(i, j) = T_C(0);
            }
        }
        // Copy P (permutation matrix)
        for (size_t i = 0; i < SIZE_T_C(ipv); ++i)
        {
            if (i == SIZE_T_C(ipiv[i] - 1)) { P(i, i) = T_C(1); }
            else
            {
                P(i, SIZE_T_C(ipiv[i] - 1)) = T_C(1);
                P(SIZE_T_C(ipiv[i] - 1), i) = T_C(1);
            }
        }
        return L;
    }

} // namespace la

#undef DOUBLE_P_R
#undef FLOAT_P_R
#undef INT_C
#undef SIZE_T_C
#undef T_C

// Explicit template instantiation
#define INSTANTIATE_LU_TEMPLATE(type)                                                                                  \
    template la::Matrix<type>& la::MatLU(la::Matrix<type>& L, la::Matrix<type>& U, la::Matrix<type>& P,                \
                                         const la::Matrix<type>& A, const int& flags);

#define INSTANTIATE_ALL_LU_TEMPLATES                                                                                   \
    INSTANTIATE_LU_TEMPLATE(float)                                                                                     \
    INSTANTIATE_LU_TEMPLATE(double)                                                                                    \
    INSTANTIATE_LU_TEMPLATE(std::complex<float>)                                                                       \
    INSTANTIATE_LU_TEMPLATE(std::complex<double>)

INSTANTIATE_ALL_LU_TEMPLATES

#undef INSTANTIATE_LU_TEMPLATE
#undef INSTANTIATE_ALL_LU_TEMPLATES
