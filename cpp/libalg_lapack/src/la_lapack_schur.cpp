/************************/
/* la_lapack_schur.cpp  */
/*    Version 1.0       */
/*     2023/06/12       */
/************************/

#include <cassert>
#include <complex>
#include <type_traits>
#include <typeinfo>
#include "la_lapack_macro.h"
#include "la_lapack_schur.h"
#include "lapack_interface.h"

#define INT_C(x)      static_cast<int>(x)
#define SIZE_T_C(x)   static_cast<size_t>(x)
#define FLOAT_P_R(x)  reinterpret_cast<float*>(x)
#define DOUBLE_P_R(x) reinterpret_cast<double*>(x)

namespace la
{

    // Helper functions to select eigenvalues
    template <typename T> int SelectEigenvaluesRealType(T* wr, T* wi)
    {
        (void)wi;
        if constexpr (std::is_same_v<T, float>) return std::abs(*wr) > 0.0f;
        else if constexpr (std::is_same_v<T, double>) return std::abs(*wr) > 0.0;
        else throw std::runtime_error("SelectEigenvaluesRealType: unsupported type");
        return 1;
    }

    template <typename T> int SelectEigenvaluesComplexType(T* w)
    {
        (void)w;
        if constexpr (std::is_same_v<T, std::complex<float>>) return (*w).real() > 0.0f;
        else if constexpr (std::is_same_v<T, std::complex<double>>) return (*w).real() > 0.0;
        else throw std::runtime_error("SelectEigenvaluesComplexType: unsupported type");
    }

    template <typename T>
    Matrix<T>& MatSchur(Matrix<T>& E, Matrix<T>* pV, Matrix<T>& S, const Matrix<T>& A, const int& flags)
    {
        (void)flags;
        assert(E.GetRowsNb() == A.GetRowsNb());
        assert(A.GetRowsNb() == A.GetColsNb());
        REALTYPE_DEFINE
        int n = INT_C(A.GetRowsNb());
        // Assign A to S so it will contains Schur form
        S     = A;
        char jobvs, sort = (flags & la::SCHUR::SORT) ? 'S' : 'N';
        T* pVTmp = nullptr;
        if (pV)
        {
            assert(pV->GetRowsNb() == A.GetRowsNb());
            assert(pV->GetColsNb() == A.GetColsNb());
            jobvs = 'V';
            pVTmp = pV->data().data();
        }
        else jobvs = 'N';
        int lda = n, ldvs = n, sdim = 0, lwork = -1, info = 0;
        std::vector<T> work(1);
        std::vector<RealType> rwork(SIZE_T_C(2 * n));
        std::vector<int> bwork(SIZE_T_C(n));
        if constexpr (std::is_same_v<T, float>)
        {
            assert(E.GetColsNb() == 2);
            int (*select)(T*, T*) = (flags & la::SCHUR::SORT) ? SelectEigenvaluesRealType<T> : nullptr;
            T *e1_ = &E.data()[0], *e2_ = &E.data()[SIZE_T_C(n)];
            sgees_(&jobvs, &sort, select, &n, S.data().data(), &lda, &sdim, e1_, e2_, pVTmp, &ldvs, work.data(), &lwork,
                   bwork.data(), &info);
            lwork = INT_C(work[0]);
            work.resize(SIZE_T_C(lwork));
            sgees_(&jobvs, &sort, select, &n, S.data().data(), &lda, &sdim, e1_, e2_, pVTmp, &ldvs, work.data(), &lwork,
                   bwork.data(), &info);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            assert(E.GetColsNb() == 2);
            int (*select)(T*, T*) = (flags & la::SCHUR::SORT) ? SelectEigenvaluesRealType<T> : nullptr;
            T *e1_ = &E.data()[0], *e2_ = &E.data()[SIZE_T_C(n)];
            dgees_(&jobvs, &sort, select, &n, S.data().data(), &lda, &sdim, e1_, e2_, pVTmp, &ldvs, work.data(), &lwork,
                   bwork.data(), &info);
            lwork = INT_C(work[0]);
            work.resize(SIZE_T_C(lwork));
            dgees_(&jobvs, &sort, select, &n, S.data().data(), &lda, &sdim, e1_, e2_, pVTmp, &ldvs, work.data(), &lwork,
                   bwork.data(), &info);
        }
        else if constexpr (std::is_same_v<T, std::complex<float>>)
        {
            assert(E.GetColsNb() == 1);
            int (*select)(T*) = (flags & la::SCHUR::SORT) ? SelectEigenvaluesComplexType<T> : nullptr;
            int (*selectFloat)(float*) =
                (flags & la::SCHUR::SORT) ? reinterpret_cast<int (*)(float*)>(select) : nullptr;
            float* pVTmpFloat = pVTmp ? FLOAT_P_R(pVTmp) : nullptr;
            cgees_(&jobvs, &sort, selectFloat, &n, FLOAT_P_R(S.data().data()), &lda, &sdim, FLOAT_P_R(E.data().data()),
                   pVTmpFloat, &ldvs, FLOAT_P_R(work.data()), &lwork, rwork.data(), bwork.data(), &info);
            lwork = INT_C(work[0].real());
            work.resize(SIZE_T_C(lwork));
            cgees_(&jobvs, &sort, selectFloat, &n, FLOAT_P_R(S.data().data()), &lda, &sdim, FLOAT_P_R(E.data().data()),
                   pVTmpFloat, &ldvs, FLOAT_P_R(work.data()), &lwork, rwork.data(), bwork.data(), &info);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>)
        {
            assert(E.GetColsNb() == 1);
            int (*select)(T*) = (flags & la::SCHUR::SORT) ? SelectEigenvaluesComplexType<T> : nullptr;
            int (*selectDouble)(double*) =
                (flags & la::SCHUR::SORT) ? reinterpret_cast<int (*)(double*)>(select) : nullptr;
            double* pVTmpDouble = pVTmp ? DOUBLE_P_R(pVTmp) : nullptr;
            zgees_(&jobvs, &sort, selectDouble, &n, DOUBLE_P_R(S.data().data()), &lda, &sdim,
                   DOUBLE_P_R(E.data().data()), pVTmpDouble, &ldvs, DOUBLE_P_R(work.data()), &lwork, rwork.data(),
                   bwork.data(), &info);
            lwork = INT_C(work[0].real());
            work.resize(SIZE_T_C(lwork));
            zgees_(&jobvs, &sort, selectDouble, &n, DOUBLE_P_R(S.data().data()), &lda, &sdim,
                   DOUBLE_P_R(E.data().data()), pVTmpDouble, &ldvs, DOUBLE_P_R(work.data()), &lwork, rwork.data(),
                   bwork.data(), &info);
        }
        else { throw std::runtime_error("MatSchur: unsupported type"); }
        if (info < 0) throw std::runtime_error("MatSchur: illegal value");
        else if (info > 0) throw std::runtime_error("MatSchur: failed to converge");
        return E;
    }

} // namespace la

#undef DOUBLE_P_R
#undef FLOAT_P_R
#undef INT_C
#undef SIZE_T_C

#define INSTANTIATE_SCHUR_TEMPLATE(type)                                                                               \
    template la::Matrix<type>& la::MatSchur<type>(la::Matrix<type>&, la::Matrix<type>*, la::Matrix<type>&,             \
                                                  const la::Matrix<type>&, const int&);

#define INSTANTIATE_ALL_SCHUR_TEMPLATES                                                                                \
    INSTANTIATE_SCHUR_TEMPLATE(float)                                                                                  \
    INSTANTIATE_SCHUR_TEMPLATE(double)                                                                                 \
    INSTANTIATE_SCHUR_TEMPLATE(std::complex<float>)                                                                    \
    INSTANTIATE_SCHUR_TEMPLATE(std::complex<double>)

INSTANTIATE_ALL_SCHUR_TEMPLATES

#undef INSTANTIATE_SCHUR_TEMPLATE
#undef INSTANTIATE_ALL_SCHUR_TEMPLATES
