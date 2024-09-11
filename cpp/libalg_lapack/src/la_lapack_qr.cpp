/************************/
/*  la_lapack_qr.cpp    */
/*    Version 1.0       */
/*     2023/06/01       */
/************************/

#include <cassert>
#include <complex>
#include <type_traits>
#include <typeinfo>
#include "la_blas_mult.h"
#include "la_lapack_qr.h"
#include "lapack_interface.h"

#define INT_C(x)      static_cast<int>(x)
#define SIZE_T_C(x)   static_cast<size_t>(x)
#define FLOAT_P_R(x)  reinterpret_cast<float*>(x)
#define DOUBLE_P_R(x) reinterpret_cast<double*>(x)

namespace la
{
    template <typename T> Matrix<T>& MatQR(Matrix<T>& Q, Matrix<T>& R, const Matrix<T>& A, const int& flags)
    {
        assert(Q.GetRowsNb() == A.GetRowsNb());
        assert(Q.GetColsNb() == A.GetRowsNb());
        assert(R.GetRowsNb() == A.GetRowsNb());
        assert(R.GetColsNb() == A.GetColsNb());
        (void)flags;
        int dimsquare = INT_C(std::max(A.GetRowsNb(), A.GetColsNb()));
        int m_orig = INT_C(A.GetRowsNb()), n_orig = INT_C(A.GetColsNb());
        int m = dimsquare, n = dimsquare;
        Matrix<T> Atmp{SIZE_T_C(m), SIZE_T_C(n)};
        // copy A in the expanded Atmp matrix
        for (size_t i = 0; i < SIZE_T_C(m_orig); ++i)
            for (size_t j = 0; j < SIZE_T_C(n_orig); ++j) Atmp(i, j) = A(i, j);
        int lda   = m;
        int lwork = -1, info = 0;
        std::vector<T> work(1), tau(SIZE_T_C(std::min(m, n)));
        if constexpr (std::is_same_v<T, float>)
        {
            sgeqrf_(&m, &n, Atmp.data().data(), &lda, tau.data(), work.data(), &lwork, &info);
            lwork = INT_C(work[0]);
            work.resize(SIZE_T_C(lwork));
            sgeqrf_(&m, &n, Atmp.data().data(), &lda, tau.data(), work.data(), &lwork, &info);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            dgeqrf_(&m, &n, Atmp.data().data(), &lda, tau.data(), work.data(), &lwork, &info);
            lwork = INT_C(work[0]);
            work.resize(SIZE_T_C(lwork));
            dgeqrf_(&m, &n, Atmp.data().data(), &lda, tau.data(), work.data(), &lwork, &info);
        }
        else if constexpr (std::is_same_v<T, std::complex<float>>)
        {
            cgeqrf_(&m, &n, FLOAT_P_R(Atmp.data().data()), &lda, FLOAT_P_R(tau.data()), FLOAT_P_R(work.data()), &lwork,
                    &info);
            lwork = INT_C(work[0].real());
            work.resize(SIZE_T_C(lwork));
            cgeqrf_(&m, &n, FLOAT_P_R(Atmp.data().data()), &lda, FLOAT_P_R(tau.data()), FLOAT_P_R(work.data()), &lwork,
                    &info);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>)
        {
            zgeqrf_(&m, &n, DOUBLE_P_R(Atmp.data().data()), &lda, DOUBLE_P_R(tau.data()), DOUBLE_P_R(work.data()),
                    &lwork, &info);
            lwork = INT_C(work[0].real());
            work.resize(SIZE_T_C(lwork));
            zgeqrf_(&m, &n, DOUBLE_P_R(Atmp.data().data()), &lda, DOUBLE_P_R(tau.data()), DOUBLE_P_R(work.data()),
                    &lwork, &info);
        }
        else { throw std::runtime_error("MatQR: type not supported"); }
        if (info < 0) throw std::runtime_error("MatQR: illegal value");
        else if (info > 0) throw std::runtime_error("MatQR: matrix is not full rank");
        // R are in the bottom right corner of Atmp as the matrix atmp is the transpose of a
        // column major matrix
        // the elements on and below the diagonal of the array
        //  contain the min(M,N)-by-N upper trapezoidal matrix R (R is upper triangular if m >= n);
        for (size_t i = 0; i < R.GetRowsNb(); ++i)
            for (size_t j = i; j < R.GetColsNb(); ++j) R(i, j) = Atmp(i, j);
        if constexpr (std::is_same_v<T, float>)
        {
            lwork = -1;
            sorgqr_(&m, &n, &n, Atmp.data().data(), &lda, tau.data(), work.data(), &lwork, &info);
            lwork = INT_C(work[0]);
            work.resize(SIZE_T_C(lwork));
            sorgqr_(&m, &n, &n, Atmp.data().data(), &lda, tau.data(), work.data(), &lwork, &info);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            lwork = -1;
            dorgqr_(&m, &n, &n, Atmp.data().data(), &lda, tau.data(), work.data(), &lwork, &info);
            lwork = INT_C(work[0]);
            work.resize(SIZE_T_C(lwork));
            dorgqr_(&m, &n, &n, Atmp.data().data(), &lda, tau.data(), work.data(), &lwork, &info);
        }
        else if constexpr (std::is_same_v<T, std::complex<float>>)
        {
            lwork = -1;
            cungqr_(&m, &n, &n, FLOAT_P_R(Atmp.data().data()), &lda, FLOAT_P_R(tau.data()), FLOAT_P_R(work.data()),
                    &lwork, &info);
            lwork = INT_C(work[0].real());
            work.resize(SIZE_T_C(lwork));
            cungqr_(&m, &n, &n, FLOAT_P_R(Atmp.data().data()), &lda, FLOAT_P_R(tau.data()), FLOAT_P_R(work.data()),
                    &lwork, &info);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>)
        {
            lwork = -1;
            zungqr_(&m, &n, &n, DOUBLE_P_R(Atmp.data().data()), &lda, DOUBLE_P_R(tau.data()), DOUBLE_P_R(work.data()),
                    &lwork, &info);
            lwork = INT_C(work[0].real());
            work.resize(SIZE_T_C(lwork));
            zungqr_(&m, &n, &n, DOUBLE_P_R(Atmp.data().data()), &lda, DOUBLE_P_R(tau.data()), DOUBLE_P_R(work.data()),
                    &lwork, &info);
        }
        else { throw std::runtime_error("MatQR: type not supported"); }
        if (info < 0) throw std::runtime_error("MatQR: illegal value");
        else if (info > 0) throw std::runtime_error("MatQR: matrix is not full rank");
        for (size_t i = 0; i < Q.GetRowsNb(); ++i)
            for (size_t j = 0; j < Q.GetRowsNb(); ++j) Q(i, j) = Atmp(i, j);
        return Q;
    }
} // namespace la

#undef DOUBLE_P_R
#undef FLOAT_P_R
#undef INT_C
#undef SIZE_T_C

// Explicit template instantiation
#define INSTANTIATE_QR_TEMPLATE(type)                                                                                  \
    template la::Matrix<type>& la::MatQR(la::Matrix<type>& Q, la::Matrix<type>& R, const la::Matrix<type>& A,          \
                                         const int& flags);

#define INSTANTIATE_ALL_QR_TEMPLATES                                                                                   \
    INSTANTIATE_QR_TEMPLATE(float)                                                                                     \
    INSTANTIATE_QR_TEMPLATE(double)                                                                                    \
    INSTANTIATE_QR_TEMPLATE(std::complex<float>)                                                                       \
    INSTANTIATE_QR_TEMPLATE(std::complex<double>)

INSTANTIATE_ALL_QR_TEMPLATES

#undef INSTANTIATE_QR_TEMPLATE
#undef INSTANTIATE_ALL_QR_TEMPLATES
