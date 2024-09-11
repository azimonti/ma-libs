/************************/
/* la_lapack_misc.cpp   */
/*    Version 1.0       */
/*     2023/06/13       */
/************************/

#include <cassert>
#include <complex>
#include <limits>
#include <type_traits>
#include <typeinfo>
#include "la_lapack_eigen.h"
#include "la_lapack_macro.h"
#include "la_lapack_misc.h"
#include "la_lapack_qr.h"

#define T_C(x)  static_cast<T>(x)
#define T_RT(x) static_cast<std::complex<RealType>>(x)

namespace la
{
    template <typename T> size_t MatRank(const Matrix<T>& A, const T& tol)
    {
        REALTYPE_DEFINE
        size_t res_ = 0;
        // Compute QR factorization of A
        int flags   = 0;
        la::MatrixQR<T> QR{A};
        QR.Compute(flags);
        RealType tol_;
        if constexpr (std::is_same<T, std::complex<double>>::value || std::is_same<T, std::complex<float>>::value)
            tol_ = tol.real();
        else tol_ = tol;
        for (size_t i = 0; i < std::min(QR.R().GetRowsNb(), QR.R().GetColsNb()); ++i)
            if (std::abs(QR.R()(i, i)) > tol_) ++res_;
        return res_;
    }

    template <typename T> T MatDet(const Matrix<T>& A)
    {
        REALTYPE_DEFINE
        assert(A.GetRowsNb() == A.GetColsNb());
        std::complex<RealType> res_ = T_RT(1);
        la::Matrix<std::complex<RealType>> eigenvalues_{A.GetRowsNb(), 1};
        la::MatrixEigen<T> Eigen{A};
        Eigen.Compute();
        for (size_t i = 0; i < A.GetRowsNb(); ++i) res_ *= Eigen.EC()(i, 0);
        if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value) return res_.real();
        else if constexpr (std::is_same<T, std::complex<float>>::value || std::is_same<T, std::complex<double>>::value)
            return res_;
        else throw std::runtime_error("MatDet: unsupported type");
    }

} // namespace la

#define INSTANTIATE_MISC_TEMPLATE(type)                                                                                \
    template size_t la::MatRank<type>(const la::Matrix<type>& A, const type& tol);                                     \
    template type la::MatDet<type>(const la::Matrix<type>& A);

#define INSTANTIATE_ALL_MISC_TEMPLATES                                                                                 \
    INSTANTIATE_MISC_TEMPLATE(float)                                                                                   \
    INSTANTIATE_MISC_TEMPLATE(double)                                                                                  \
    INSTANTIATE_MISC_TEMPLATE(std::complex<float>)                                                                     \
    INSTANTIATE_MISC_TEMPLATE(std::complex<double>)

INSTANTIATE_ALL_MISC_TEMPLATES

#undef INSTANTIATE_MISC_TEMPLATE
#undef INSTANTIATE_ALL_MISC_TEMPLATES
#undef T_RT
#undef T_C
