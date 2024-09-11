#ifndef _LA_LAPACK_SVD_H_75EC0980316E424B8BA046049B60E32A_
#define _LA_LAPACK_SVD_H_75EC0980316E424B8BA046049B60E32A_

/**********************/
/*  la_lapack_svd.h   */
/*   Version 1.0      */
/*   2023/03/26       */
/**********************/

#ifndef USE_LAPACK
#error "USE_LAPACK is not defined"
#endif

#include "la_blas_mult.h"
#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"

namespace la
{
    namespace DRIVER
    {
        enum Flags : int {
            GESVD = 1,
            GESDD = 2,
            GESVJ = 3,
            GEJSV = 4,
        };
    }

    namespace SVD
    {
        enum Flags : int {
            V_HT       = 1 << 0,
            COMPLETE_U = 1 << 1,
            COMPLETE_V = 1 << 2,
        };
    }

    // compute the SVD decomposition
    // A = U * S * V'
    // U and V are orthogonal matrices
    // S is a diagonal matrix
    // the singular values are stored in the diagonal of S
    // the singular values are sorted in descending order
    // the columns of U and V are the left and right singular vectors
    // the columns of U and V are sorted in descending order of the singular values
    template <typename T>
    Matrix<T>& MatSVD(Matrix<T>& U, Matrix<T>& S, Matrix<T>& V, const Matrix<T>& A, const int& DRIVER = 1,
                      const int& flags = false);

    template <typename T>
    Matrix<T> MatSVD(Matrix<T>& S, Matrix<T>& V, const Matrix<T>& A, const int& DRIVER, const int& flags)
    {
        Matrix<T> U{A.GetRowsNb(), A.GetColsNb()};
        return MatSVD(U, S, V, A, DRIVER, flags);
    }

    template <typename T> Matrix<T> MatSVD(Matrix<T>& S, const Matrix<T>& A, const int& DRIVER, const int& flags)
    {
        Matrix<T> U{A.GetRowsNb(), A.GetColsNb()};
        Matrix<T> V{A.GetColsNb(), A.GetColsNb()};
        return MatSVD(U, S, V, A, DRIVER, flags);
    }

    template <typename T> Matrix<T> MatSVD(const Matrix<T>& A, const int& DRIVER, const int& flags)
    {
        Matrix<T> U{A.GetRowsNb(), A.GetColsNb()};
        Matrix<T> S{A.GetColsNb(), A.GetColsNb()};
        Matrix<T> V{A.GetColsNb(), A.GetColsNb()};
        return MatSVD(U, S, V, A, DRIVER, flags);
    }

    template <typename T> class MatrixSVD
    {

      public:
        inline MatrixSVD(const Matrix<T>& M)
            : A_(M), U_(M.GetRowsNb(), M.GetRowsNb()), S_(M.GetRowsNb(), M.GetColsNb()),
              V_(M.GetColsNb(), M.GetColsNb()), C_(0, 0), DRIVER_(0), flags_(0)
        {
        }

        inline const Matrix<T>& A() const { return A_; }

        inline const Matrix<T>& U() const { return U_; }

        inline const Matrix<T>& S() const { return S_; }

        inline const Matrix<T>& V() const { return V_; }

        inline int DRIVER() const { return DRIVER_; }

        inline int flags() const { return flags_; }

        inline void Compute(const int& DRIVER, const int& flags = 0)
        {
            flags_  = flags;
            DRIVER_ = DRIVER;
            MatSVD(U_, S_, V_, A_, DRIVER_, flags_);
        }

        inline const Matrix<T>& C()
        {
            if (C_.size() == 0)
            {
                C_ = Matrix<T>(U_.GetRowsNb(), V_.GetColsNb());
                if (flags_ & la::SVD::V_HT) MatMult(C_, U_, MatMult(S_, V_));
                else MatMult(C_, U_, MatMult(S_, MatHermitian(V_)));
            }
            return C_;
        }

        inline int GetFlags() const { return flags_; }

      private:
        const Matrix<T>& A_;
        Matrix<T> U_;
        Matrix<T> S_;
        Matrix<T> V_;
        Matrix<T> C_;
        int DRIVER_;
        int flags_;
    };

} // namespace la

#endif
