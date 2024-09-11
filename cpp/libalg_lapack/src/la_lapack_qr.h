#ifndef _LA_LAPACK_QR_H_AC9F890E52F542D4A5F236398520F6EC_
#define _LA_LAPACK_QR_H_AC9F890E52F542D4A5F236398520F6EC_

/************************/
/*    la_lapack_qr.h    */
/*    Version 1.0       */
/*     2023/06/01       */
/************************/

#ifndef USE_LAPACK
#error "USE_LAPACK is not defined"
#endif

#include "la_blas_mult.h"
#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"

namespace la
{
    namespace QR
    {
        enum Flags : int {
            // any flag for future use
            // FLAG1 = 1 << 0,
        };
    }

    // Compute the QR decomposition of a matrix A
    //    A = Q * ( R ),
    //            ( 0 )
    // A is a M-by-N matrix;
    // Q is a M-by-M orthogonal matrix;
    // R is an upper-triangular N-by-N matrix; 0 is a (M-N)-by-N zero matrix, if M > N. It is stored within R.
    // R is an upper-triangular N-by-M matrix if M < N.
    // The columns of Q are the left orthogonal vectors
    // The columns of Q are normalized
    template <typename T> Matrix<T>& MatQR(Matrix<T>& Q, Matrix<T>& R, const Matrix<T>& A, const int& flags = 0);

    template <typename T> Matrix<T> MatQR(Matrix<T>& R, const Matrix<T>& A, const int& flags)
    {
        Matrix<T> Q{A.GetRowsNb(), A.GetRowsNb()};
        // return Q
        return MatQR(Q, R, A, flags);
    }

    template <typename T> Matrix<T> MatQR(const Matrix<T>& A, const int& flags)
    {
        Matrix<T> Q{A.GetRowsNb(), A.GetRowsNb()};
        Matrix<T> R{A.GetRowsNb(), A.GetColsNb()};
        // return Q
        return MatQR(Q, R, A, flags);
    }

    template <typename T> class MatrixQR
    {
      public:
        inline MatrixQR(const Matrix<T>& M)
            : A_(M), Q_(M.GetRowsNb(), M.GetRowsNb()), R_(M.GetRowsNb(), M.GetColsNb()), C_(0, 0), flags_(0)
        {
        }

        inline const Matrix<T>& A() const { return A_; }

        inline const Matrix<T>& Q() const { return Q_; }

        inline const Matrix<T>& R() const { return R_; }

        inline void Compute(const int& flags = 0)
        {
            flags_ = flags;
            MatQR(Q_, R_, A_, flags_);
        }

        inline const Matrix<T>& C()
        {
            if (C_.size() == 0)
            {
                C_ = Matrix<T>(Q_.GetRowsNb(), R_.GetColsNb());
                MatMult(C_, Q_, R_);
            }
            return C_;
        }

      private:
        const Matrix<T>& A_;
        Matrix<T> Q_;
        Matrix<T> R_;
        Matrix<T> C_;
        int flags_;
    };
} // namespace la

#endif
