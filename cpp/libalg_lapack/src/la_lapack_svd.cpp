/**********************/
/* la_lapack_svd.cpp  */
/*   Version 1.0      */
/*   2023/03/26       */
/**********************/

#include <cassert>
#include <complex>
#include <type_traits>
#include <typeinfo>
#include "la_blas_mult.h"
#include "la_lapack_macro.h"
#include "la_lapack_qr.h"
#include "la_lapack_svd.h"
#include "lapack_interface.h"

#define INT_C(x)      static_cast<int>(x)
#define SIZE_T_C(x)   static_cast<size_t>(x)
#define FLOAT_P_R(x)  reinterpret_cast<float*>(x)
#define DOUBLE_P_R(x) reinterpret_cast<double*>(x)

namespace la
{
    template <typename T>
    Matrix<T>& MatSVD(Matrix<T>& U, Matrix<T>& S, Matrix<T>& V, const Matrix<T>& A, const int& DRIVER, const int& flags)
    {
        assert(U.GetRowsNb() == A.GetRowsNb());
        assert(U.GetColsNb() == A.GetRowsNb());
        assert(S.GetRowsNb() == A.GetRowsNb());
        assert(S.GetColsNb() == A.GetColsNb());
        assert(V.GetRowsNb() == A.GetColsNb());
        assert(V.GetColsNb() == A.GetColsNb());
        REALTYPE_DEFINE
        int m = INT_C(A.GetRowsNb()), n = INT_C(A.GetColsNb());
        Matrix<T> Atmp = A;
        if ((DRIVER == la::DRIVER::GESVJ || DRIVER == la::DRIVER::GEJSV) && n > m)
        {
            Atmp.Transpose();
            Matrix<T> Stmp = Matrix<T>{SIZE_T_C(n), SIZE_T_C(m)};
            // Compute the SVD of the transpose of A V_HT is purposely not se
            int flagsTmp   = 0;
            if (flags & la::SVD::COMPLETE_U) flagsTmp |= la::SVD::COMPLETE_V;
            if (flags & la::SVD::COMPLETE_V) flagsTmp |= la::SVD::COMPLETE_U;

            MatSVD(V, Stmp, U, Atmp, DRIVER, flagsTmp);
            // S is the transpose of Stmp
            MatTranspose(S, Stmp);
            // U is the transpose conjugate
            U.Conjugate();
            //  V is the transpose conjugate
            if (flags & la::SVD::V_HT) V.Transpose();
            else V.Conjugate();
            return S;
        }
        char jobu = 'A';
        if (DRIVER == la::DRIVER::GESVJ || DRIVER == la::DRIVER::GEJSV) jobu = 'U';
        int lda = m, ldu = m, ldvt = n;
        int lwork = -1, lrwork, info;
        std::vector<int> iwork(8 * SIZE_T_C(std::min(m, n)));
        std::vector<T> work;
        T wkopt;
        // RealType is used because lapack is using float and double for complex function call
        std::vector<RealType> Stmp(SIZE_T_C(std::min(m, n))), rwork;
        const size_t mn = SIZE_T_C(std::min(m, n)), mx = SIZE_T_C(std::max(m, n));
        // the following are only relevant for GESVJ OR GEJSV
        char joba = 'G';
        char jobv = 'V';
        char jobr = 'R';
        char jobt = 'N';
        char jobp = 'N';
        int mv    = INT_C(std::max(m, n));
        if constexpr (std::is_same_v<T, float>)
        {
            switch (DRIVER)
            {
            case DRIVER::GESVD:
                // call to get optimal work size
                sgesvd_(&jobu, &jobu, &m, &n, Atmp.data().data(), &lda, Stmp.data(), U.data().data(), &ldu,
                        V.data().data(), &ldvt, &wkopt, &lwork, &info);
                // allocate work
                lwork = INT_C(wkopt);
                work.resize(SIZE_T_C(lwork));
                // call lapack
                sgesvd_(&jobu, &jobu, &m, &n, Atmp.data().data(), &lda, Stmp.data(), U.data().data(), &ldu,
                        V.data().data(), &ldvt, work.data(), &lwork, &info);
                // Since LAPACK stores the singular values in a vector, we need to convert it to a diagonal matrix
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            case DRIVER::GESDD:
                sgesdd_(&jobu, &m, &n, Atmp.data().data(), &lda, Stmp.data(), U.data().data(), &ldu, V.data().data(),
                        &ldvt, &wkopt, &lwork, iwork.data(), &info);
                lwork = INT_C(wkopt);
                work.resize(SIZE_T_C(lwork));
                sgesdd_(&jobu, &m, &n, Atmp.data().data(), &lda, Stmp.data(), U.data().data(), &ldu, V.data().data(),
                        &ldvt, work.data(), &lwork, iwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            case DRIVER::GESVJ:
                lwork = std::max(6, m + n);
                work.resize(SIZE_T_C(lwork));
                sgesvj_(&joba, &jobu, &jobv, &m, &n, Atmp.data().data(), &lda, Stmp.data(), &mv, V.data().data(), &ldvt,
                        work.data(), &lwork, &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                // U are stored in the first m rows of Atmp
                for (size_t i = 0; i < SIZE_T_C(m); i++)
                    for (size_t j = 0; j < SIZE_T_C(n); j++) U(i, j) = Atmp(i, j);
                break;
            case DRIVER::GEJSV:
                lwork = std::max(2 * m + n, 6 * n + 2 * n * n);
                work.resize(SIZE_T_C(lwork));
                sgejsv_(&joba, &jobu, &jobv, &jobr, &jobt, &jobp, &m, &n, Atmp.data().data(), &lda, Stmp.data(),
                        U.data().data(), &ldu, V.data().data(), &ldvt, work.data(), &lwork, iwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            default:
                // throw runtime error
                throw std::runtime_error("MatSVD: unsupported DRIVER");
            }
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            switch (DRIVER)
            {
            case DRIVER::GESVD:
                dgesvd_(&jobu, &jobu, &m, &n, Atmp.data().data(), &lda, Stmp.data(), U.data().data(), &ldu,
                        V.data().data(), &ldvt, &wkopt, &lwork, &info);
                lwork = INT_C(wkopt);
                work.resize(SIZE_T_C(lwork));
                dgesvd_(&jobu, &jobu, &m, &n, Atmp.data().data(), &lda, Stmp.data(), U.data().data(), &ldu,
                        V.data().data(), &ldvt, work.data(), &lwork, &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            case DRIVER::GESDD:
                dgesdd_(&jobu, &m, &n, Atmp.data().data(), &lda, Stmp.data(), U.data().data(), &ldu, V.data().data(),
                        &ldvt, &wkopt, &lwork, iwork.data(), &info);
                lwork = INT_C(wkopt);
                work.resize(SIZE_T_C(lwork));
                dgesdd_(&jobu, &m, &n, Atmp.data().data(), &lda, Stmp.data(), U.data().data(), &ldu, V.data().data(),
                        &ldvt, work.data(), &lwork, iwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            case DRIVER::GESVJ:
                lwork = std::max(6, m + n);
                work.resize(SIZE_T_C(lwork));
                dgesvj_(&joba, &jobu, &jobv, &m, &n, Atmp.data().data(), &lda, Stmp.data(), &mv, V.data().data(), &ldvt,
                        work.data(), &lwork, &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                for (size_t i = 0; i < SIZE_T_C(m); i++)
                    for (size_t j = 0; j < SIZE_T_C(n); j++) U(i, j) = Atmp(i, j);
                break;
            case DRIVER::GEJSV:
                lwork = std::max(2 * m + n, 6 * n + 2 * n * n);
                work.resize(SIZE_T_C(lwork));
                dgejsv_(&joba, &jobu, &jobv, &jobr, &jobt, &jobp, &m, &n, Atmp.data().data(), &lda, Stmp.data(),
                        U.data().data(), &ldu, V.data().data(), &ldvt, work.data(), &lwork, iwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            default:
                // throw runtime error
                throw std::runtime_error("MatSVD: unsupported DRIVER");
            }
        }
        // complex float
        else if constexpr (std::is_same_v<T, std::complex<float>>)
        {
            switch (DRIVER)
            {
            case DRIVER::GESVD:
                rwork.resize(5 * SIZE_T_C(std::min(m, n)));
                cgesvd_(&jobu, &jobu, &m, &n, FLOAT_P_R(Atmp.data().data()), &lda, Stmp.data(),
                        FLOAT_P_R(U.data().data()), &ldu, FLOAT_P_R(V.data().data()), &ldvt, FLOAT_P_R(&wkopt), &lwork,
                        rwork.data(), &info);
                lwork = INT_C(wkopt.real());
                work.resize(SIZE_T_C(lwork));
                cgesvd_(&jobu, &jobu, &m, &n, FLOAT_P_R(Atmp.data().data()), &lda, Stmp.data(),
                        FLOAT_P_R(U.data().data()), &ldu, FLOAT_P_R(V.data().data()), &ldvt, FLOAT_P_R(work.data()),
                        &lwork, rwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            case DRIVER::GESDD:
                rwork.resize(SIZE_T_C(std::max(5 * mn * mn + 5 * mn, 2 * mx * mn + 2 * mn * mn + mn)));
                cgesdd_(&jobu, &m, &n, FLOAT_P_R(Atmp.data().data()), &lda, Stmp.data(), FLOAT_P_R(U.data().data()),
                        &ldu, FLOAT_P_R(V.data().data()), &ldvt, FLOAT_P_R(&wkopt), &lwork, rwork.data(), iwork.data(),
                        &info);
                lwork = INT_C(wkopt.real());
                work.resize(SIZE_T_C(lwork));
                cgesdd_(&jobu, &m, &n, FLOAT_P_R(Atmp.data().data()), &lda, Stmp.data(), FLOAT_P_R(U.data().data()),
                        &ldu, FLOAT_P_R(V.data().data()), &ldvt, FLOAT_P_R(work.data()), &lwork, rwork.data(),
                        iwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            case DRIVER::GESVJ:
                lwork = m + n;
                work.resize(SIZE_T_C(lwork));
                lrwork = std::max(6, m + n);
                rwork.resize(SIZE_T_C(lrwork));
                cgesvj_(&joba, &jobu, &jobv, &m, &n, FLOAT_P_R(Atmp.data().data()), &lda, Stmp.data(), &mv,
                        FLOAT_P_R(V.data().data()), &ldvt, FLOAT_P_R(work.data()), &lwork, rwork.data(), &lrwork,
                        &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                for (size_t i = 0; i < SIZE_T_C(m); i++)
                    for (size_t j = 0; j < SIZE_T_C(n); j++) U(i, j) = Atmp(i, j);
                break;
            case DRIVER::GEJSV:
                lwork  = -1;
                lrwork = -1;
                rwork.resize(1);
                work.resize(2);
                cgejsv_(&joba, &jobu, &jobv, &jobr, &jobt, &jobp, &m, &n, FLOAT_P_R(Atmp.data().data()), &lda,
                        Stmp.data(), FLOAT_P_R(U.data().data()), &ldu, FLOAT_P_R(V.data().data()), &ldvt,
                        FLOAT_P_R(work.data()), &lwork, rwork.data(), &lrwork, iwork.data(), &info);
                lrwork = INT_C(rwork[0]);
                rwork.resize(SIZE_T_C(lrwork));
                lwork = INT_C(work[0].real());
                work.resize(SIZE_T_C(lwork));
                cgejsv_(&joba, &jobu, &jobv, &jobr, &jobt, &jobp, &m, &n, FLOAT_P_R(Atmp.data().data()), &lda,
                        Stmp.data(), FLOAT_P_R(U.data().data()), &ldu, FLOAT_P_R(V.data().data()), &ldvt,
                        FLOAT_P_R(work.data()), &lwork, FLOAT_P_R(rwork.data()), &lrwork, iwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            }
        }
        // complex double
        else if constexpr (std::is_same_v<T, std::complex<double>>)
        {
            switch (DRIVER)
            {
            case DRIVER::GESVD:
                rwork.resize(5 * SIZE_T_C(std::min(m, n)));
                zgesvd_(&jobu, &jobu, &m, &n, DOUBLE_P_R(Atmp.data().data()), &lda, Stmp.data(),
                        DOUBLE_P_R(U.data().data()), &ldu, DOUBLE_P_R(V.data().data()), &ldvt, DOUBLE_P_R(&wkopt),
                        &lwork, rwork.data(), &info);
                lwork = INT_C(wkopt.real());
                work.resize(SIZE_T_C(lwork));
                zgesvd_(&jobu, &jobu, &m, &n, DOUBLE_P_R(Atmp.data().data()), &lda, Stmp.data(),
                        DOUBLE_P_R(U.data().data()), &ldu, DOUBLE_P_R(V.data().data()), &ldvt, DOUBLE_P_R(work.data()),
                        &lwork, rwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            case DRIVER::GESDD:
                rwork.resize(SIZE_T_C(std::max(5 * mn * mn + 5 * mn, 2 * mx * mn + 2 * mn * mn + mn)));
                zgesdd_(&jobu, &m, &n, DOUBLE_P_R(Atmp.data().data()), &lda, Stmp.data(), DOUBLE_P_R(U.data().data()),
                        &ldu, DOUBLE_P_R(V.data().data()), &ldvt, DOUBLE_P_R(&wkopt), &lwork, rwork.data(),
                        iwork.data(), &info);
                lwork = INT_C(wkopt.real());
                work.resize(SIZE_T_C(lwork));
                zgesdd_(&jobu, &m, &n, DOUBLE_P_R(Atmp.data().data()), &lda, Stmp.data(), DOUBLE_P_R(U.data().data()),
                        &ldu, DOUBLE_P_R(V.data().data()), &ldvt, DOUBLE_P_R(work.data()), &lwork, rwork.data(),
                        iwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            case DRIVER::GESVJ:
                lwork = m + n;
                work.resize(SIZE_T_C(lwork));
                lrwork = std::max(6, m + n);
                rwork.resize(SIZE_T_C(lrwork));
                zgesvj_(&joba, &jobu, &jobv, &m, &n, DOUBLE_P_R(Atmp.data().data()), &lda, Stmp.data(), &mv,
                        DOUBLE_P_R(V.data().data()), &ldvt, DOUBLE_P_R(work.data()), &lwork, DOUBLE_P_R(rwork.data()),
                        &lrwork, &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                for (size_t i = 0; i < SIZE_T_C(m); i++)
                    for (size_t j = 0; j < SIZE_T_C(n); j++) U(i, j) = Atmp(i, j);
                break;
            case DRIVER::GEJSV:
                lwork  = -1;
                lrwork = -1;
                rwork.resize(1);
                work.resize(2);
                zgejsv_(&joba, &jobu, &jobv, &jobr, &jobt, &jobp, &m, &n, DOUBLE_P_R(Atmp.data().data()), &lda,
                        Stmp.data(), DOUBLE_P_R(U.data().data()), &ldu, DOUBLE_P_R(V.data().data()), &ldvt,
                        DOUBLE_P_R(work.data()), &lwork, rwork.data(), &lrwork, iwork.data(), &info);
                lrwork = INT_C(rwork[0]);
                rwork.resize(SIZE_T_C(lrwork));
                lwork = INT_C(work[0].real());
                work.resize(SIZE_T_C(lwork));
                zgejsv_(&joba, &jobu, &jobv, &jobr, &jobt, &jobp, &m, &n, DOUBLE_P_R(Atmp.data().data()), &lda,
                        Stmp.data(), DOUBLE_P_R(U.data().data()), &ldu, DOUBLE_P_R(V.data().data()), &ldvt,
                        DOUBLE_P_R(work.data()), &lwork, DOUBLE_P_R(rwork.data()), &lrwork, iwork.data(), &info);
                for (size_t i = 0; i < Stmp.size(); i++) S(i, i) = Stmp[i];
                break;
            }
        }
        else { throw std::runtime_error("MatSVD: unsupported type"); }
        if (info < 0) { throw std::runtime_error("MatSVD: illegal value"); }
        else if (info > 0) { throw std::runtime_error("MatSVD: convergence not reached"); }
        if (DRIVER == DRIVER::GESVJ || (DRIVER == DRIVER::GEJSV))
        {
            if ((flags & la::SVD::COMPLETE_U) && (A.GetColsNb() < U.GetColsNb()))
            {
                // compute QR decomposition of U
                Matrix<T> Utmp{U}, Q{U.GetRowsNb(), U.GetColsNb()}, R{U.GetColsNb(), U.GetColsNb()};
                la::MatQR(Q, R, Utmp);
                for (size_t i = 0; i < U.GetColsNb(); i++)
                    for (size_t j = 0; j < U.GetRowsNb(); j++) U(i, j) = (DRIVER == DRIVER::GESVJ) ? -Q(i, j) : Q(i, j);
            }
            if (flags & la::SVD::COMPLETE_V && (A.GetColsNb() < V.GetColsNb()))
            {
                // compute QR decomposition of V
                throw std::runtime_error("MatSVD: COMPLETE_V not tested for GESVJ and GEJSV");
                Matrix<T> Vtmp{V}, Q{V.GetRowsNb(), V.GetColsNb()}, R{V.GetColsNb(), V.GetColsNb()};
                la::MatQR(Q, R, Vtmp);
                for (size_t i = 0; i < V.GetColsNb(); i++)
                    for (size_t j = 0; j < V.GetRowsNb(); j++) V(i, j) = (DRIVER == DRIVER::GESVJ) ? -Q(i, j) : Q(i, j);
            }
            // This driver return V
            if (flags & la::SVD::V_HT) V.Hermitian();
        }
        else
        {
            if (!(flags & la::SVD::V_HT)) V.Hermitian();
        }
        return S;
    }
} // namespace la

#undef DOUBLE_P_R
#undef FLOAT_P_R
#undef INT_C
#undef SIZE_T_C

// Explicit template instantiation
#define INSTANTIATE_SVD_TEMPLATE(type)                                                                                 \
    template la::Matrix<type>& la::MatSVD<type>(la::Matrix<type> & U, la::Matrix<type> & S, la::Matrix<type> & V,      \
                                                const la::Matrix<type>& A, const int& DRIVER, const int& flags);

#define INSTANTIATE_ALL_SVD_TEMPLATES                                                                                  \
    INSTANTIATE_SVD_TEMPLATE(float)                                                                                    \
    INSTANTIATE_SVD_TEMPLATE(double)                                                                                   \
    INSTANTIATE_SVD_TEMPLATE(std::complex<float>)                                                                      \
    INSTANTIATE_SVD_TEMPLATE(std::complex<double>)

INSTANTIATE_ALL_SVD_TEMPLATES

#undef INSTANTIATE_SVD_TEMPLATE
#undef INSTANTIATE_ALL_SVD_TEMPLATES
