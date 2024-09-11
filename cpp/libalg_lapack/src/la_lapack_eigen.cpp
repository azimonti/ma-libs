/************************/
/* la_lapack_eigen.cpp  */
/*    Version 1.0       */
/*     2023/06/11       */
/************************/

#include <cassert>
#include <complex>
#include <type_traits>
#include <typeinfo>
#include "la_lapack_eigen.h"
#include "la_lapack_macro.h"
#include "lapack_interface.h"

#define INT_C(x)      static_cast<int>(x)
#define SIZE_T_C(x)   static_cast<size_t>(x)
#define FLOAT_P_R(x)  reinterpret_cast<float*>(x)
#define DOUBLE_P_R(x) reinterpret_cast<double*>(x)

namespace la
{
    template <typename T>
    Matrix<T>& MatEigen(Matrix<T>& E, Matrix<T>* pVL, Matrix<T>* pVR, const Matrix<T>& A, const int& flags)
    {
        (void)flags;
        assert(E.GetRowsNb() == A.GetRowsNb());
        assert(A.GetRowsNb() == A.GetColsNb());
        REALTYPE_DEFINE
        int n          = INT_C(A.GetRowsNb());
        Matrix<T> Atmp = A;
        char jobvl     = 'N', jobvr;
        T *pVLTmp = nullptr, *pVRTmp = nullptr;
        if (pVL)
        {
            assert(pVL->GetRowsNb() == A.GetRowsNb());
            assert(pVL->GetColsNb() == A.GetColsNb());
            jobvl  = 'V';
            pVLTmp = pVL->data().data();
        }
        if (pVR)
        {
            assert(pVR->GetRowsNb() == A.GetRowsNb());
            assert(pVR->GetColsNb() == A.GetColsNb());
            jobvr  = 'V';
            pVRTmp = pVR->data().data();
        }
        else jobvr = 'N';
        int lda = n, ldvl = n, ldvr = n, lwork = -1, info = 0;
        std::vector<T> work(1);
        std::vector<RealType> rwork(SIZE_T_C(2 * n));
        if constexpr (std::is_same_v<T, float>)
        {
            T *e1_ = &E.data()[0], *e2_ = &E.data()[SIZE_T_C(n)];
            assert(E.GetColsNb() == 2);
            sgeev_(&jobvl, &jobvr, &n, Atmp.data().data(), &lda, e1_, e2_, pVLTmp, &ldvl, pVRTmp, &ldvr, work.data(),
                   &lwork, &info);
            lwork = INT_C(work[0]);
            work.resize(SIZE_T_C(lwork));
            sgeev_(&jobvl, &jobvr, &n, Atmp.data().data(), &lda, e1_, e2_, pVLTmp, &ldvl, pVRTmp, &ldvr, work.data(),
                   &lwork, &info);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            assert(E.GetColsNb() == 2);
            T *e1_ = &E.data()[0], *e2_ = &E.data()[SIZE_T_C(n)];
            dgeev_(&jobvl, &jobvr, &n, Atmp.data().data(), &lda, e1_, e2_, pVLTmp, &ldvl, pVRTmp, &ldvr, work.data(),
                   &lwork, &info);
            lwork = INT_C(work[0]);
            work.resize(SIZE_T_C(lwork));
            dgeev_(&jobvl, &jobvr, &n, Atmp.data().data(), &lda, e1_, e2_, pVLTmp, &ldvl, pVRTmp, &ldvr, work.data(),
                   &lwork, &info);
        }
        else if constexpr (std::is_same_v<T, std::complex<float>>)
        {
            assert(E.GetColsNb() == 1);
            float* pVLTmpFloat = pVLTmp ? FLOAT_P_R(pVLTmp) : nullptr;
            float* pVRTmpFloat = pVRTmp ? FLOAT_P_R(pVRTmp) : nullptr;
            cgeev_(&jobvl, &jobvr, &n, FLOAT_P_R(Atmp.data().data()), &lda, FLOAT_P_R(E.data().data()), pVLTmpFloat,
                   &ldvl, pVRTmpFloat, &ldvr, FLOAT_P_R(work.data()), &lwork, rwork.data(), &info);
            lwork = INT_C(work[0].real());
            work.resize(SIZE_T_C(lwork));
            cgeev_(&jobvl, &jobvr, &n, FLOAT_P_R(Atmp.data().data()), &lda, FLOAT_P_R(E.data().data()), pVLTmpFloat,
                   &ldvl, pVRTmpFloat, &ldvr, FLOAT_P_R(work.data()), &lwork, rwork.data(), &info);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>)
        {
            assert(E.GetColsNb() == 1);
            double* pVLTmpDouble = pVLTmp ? DOUBLE_P_R(pVLTmp) : nullptr;
            double* pVRTmpDouble = pVRTmp ? DOUBLE_P_R(pVRTmp) : nullptr;
            zgeev_(&jobvl, &jobvr, &n, DOUBLE_P_R(Atmp.data().data()), &lda, DOUBLE_P_R(E.data().data()), pVLTmpDouble,
                   &ldvl, pVRTmpDouble, &ldvr, DOUBLE_P_R(work.data()), &lwork, rwork.data(), &info);
            lwork = INT_C(work[0].real());
            work.resize(SIZE_T_C(lwork));
            zgeev_(&jobvl, &jobvr, &n, DOUBLE_P_R(Atmp.data().data()), &lda, DOUBLE_P_R(E.data().data()), pVLTmpDouble,
                   &ldvl, pVRTmpDouble, &ldvr, DOUBLE_P_R(work.data()), &lwork, rwork.data(), &info);
        }
        else { throw std::runtime_error("MatEigen: unsupported type"); }
        if (info < 0) throw std::runtime_error("MatEigen: illegal value");
        else if (info > 0) throw std::runtime_error("MatEigen: failed to converge");
        return E;
    }

} // namespace la

#undef DOUBLE_P_R
#undef FLOAT_P_R
#undef INT_C
#undef SIZE_T_C

#define INSTANTIATE_EIGEN_TEMPLATE(type)                                                                               \
    template la::Matrix<type>& la::MatEigen<type>(la::Matrix<type>&, la::Matrix<type>*, la::Matrix<type>*,             \
                                                  const la::Matrix<type>&, const int&);

#define INSTANTIATE_ALL_EIGEN_TEMPLATES                                                                                \
    INSTANTIATE_EIGEN_TEMPLATE(float)                                                                                  \
    INSTANTIATE_EIGEN_TEMPLATE(double)                                                                                 \
    INSTANTIATE_EIGEN_TEMPLATE(std::complex<float>)                                                                    \
    INSTANTIATE_EIGEN_TEMPLATE(std::complex<double>)

INSTANTIATE_ALL_EIGEN_TEMPLATES

#undef INSTANTIATE_EIGEN_TEMPLATE
#undef INSTANTIATE_ALL_EIGEN_TEMPLATES
