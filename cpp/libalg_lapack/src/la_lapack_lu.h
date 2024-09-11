#ifndef _LA_LAPACK_LU_H_115371F98FC345D7B4568537B58396AB_
#define _LA_LAPACK_LU_H_115371F98FC345D7B4568537B58396AB_

/************************/
/*   la_lapack_lu.h     */
/*    Version 1.0       */
/*     2023/06/13       */
/************************/

#ifndef USE_LAPACK
#error "USE_LAPACK is not defined"
#endif

#include "la_blas_mult.h"
#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"

namespace la
{
    namespace LU
    {
        enum Flags : int {
            // any flag for future use
            // FLAG1 = 1 << 0,
        };
    }

    // Compute the LU factorization of a general M-by-N matrix A using partial pivoting with row interchanges.
    // The factorization has the form A = P * L * U, where P is a permutation matrix, L is lower triangular with unit
    // diagonal elements (lower trapezoidal if m > n), and U is upper triangular (upper trapezoidal if m < n).
    template <typename T>
    Matrix<T>& MatLU(Matrix<T>& L, Matrix<T>& U, Matrix<T>& P, const Matrix<T>& A, const int& flags = 0);

    template <typename T> class MatrixLU
    {
      public:
        inline MatrixLU(const Matrix<T>& M)
            : A_(M), L_(0, 0), U_(0, 0), P_(M.GetColsNb(), M.GetColsNb()), C_(0, 0), flags_(0)
        {
            if (M.GetRowsNb() < M.GetColsNb())
            {
                L_ = Matrix<T>(M.GetRowsNb(), M.GetRowsNb());
                U_ = Matrix<T>(M.GetRowsNb(), M.GetColsNb());
            }
            else
            {
                L_ = Matrix<T>(M.GetRowsNb(), M.GetColsNb());
                U_ = Matrix<T>(M.GetColsNb(), M.GetColsNb());
            }
        }

        inline const Matrix<T>& A() const { return A_; }

        inline const Matrix<T>& P() const { return P_; }

        inline const Matrix<T>& L() const { return L_; }

        inline const Matrix<T>& U() const { return U_; }

        inline void Compute(const int& flags = 0)
        {
            flags_ = flags;
            MatLU(L_, U_, P_, A_, flags_);
        }

        inline const Matrix<T>& C()
        {
            if (C_.size() == 0)
            {
                C_ = Matrix<T>(P_.GetRowsNb(), U_.GetColsNb());
                MatMult(C_, P_, MatMult(L_, U_));
            }
            return C_;
        }

      private:
        const Matrix<T>& A_;
        Matrix<T> L_;
        Matrix<T> U_;
        Matrix<T> P_;
        Matrix<T> C_;
        int flags_;
    };
} // namespace la

#endif
