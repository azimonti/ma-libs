#ifndef _LA_LAPACK_SCHUR_H_DE9C7EE03B6040AEBB7A5BF396B8AB3D_
#define _LA_LAPACK_SCHUR_H_DE9C7EE03B6040AEBB7A5BF396B8AB3D_

/************************/
/*  la_lapack_schur.h   */
/*    Version 1.0       */
/*     2023/06/12       */
/************************/

#ifndef USE_LAPACK
#error "USE_LAPACK is not defined"
#endif

#include "la_blas_mult.h"
#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"

namespace la
{

    namespace SCHUR
    {
        enum Flags : int {
            SORT      = 1 << 0,
            COMPUTE_V = 1 << 1,
        };
    }

    template <typename T>
    Matrix<T>& MatSchur(Matrix<T>& E, Matrix<T>* pV, Matrix<T>& S, const Matrix<T>& A, const int& flags = 0);

    template <typename T> Matrix<T>& MatSchur(Matrix<T>& E, Matrix<T>& S, const Matrix<T>& A, const int& flags = 0)
    {
        return MatSchur<T>(E, nullptr, S, A, flags);
    }

    template <typename T> class MatrixSchur
    {
      public:
        inline MatrixSchur(const Matrix<T>& A)
            : A_(A), S_(A.GetRowsNb(), A.GetColsNb()), V_(A.GetRowsNb(), A.GetColsNb()), C_(0, 0), flags_(0)
        {
            if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
            {
                E_ = Matrix<T>(A.GetRowsNb(), 2);
            }
            else if constexpr (std::is_same<T, std::complex<float>>::value ||
                               std::is_same<T, std::complex<double>>::value)
            {
                E_ = Matrix<T>(A.GetRowsNb(), 1);
            }
            else { throw std::invalid_argument("MatrixSchur: invalid type"); }
        }

        inline const Matrix<T>& A() const { return A_; }

        inline const Matrix<T>& E() const { return E_; }

        inline const Matrix<T>& S() const { return S_; }

        inline const Matrix<T>& V() const { return V_; }

        inline const Matrix<T>& C()
        {
            if (C_.size() == 0)
            {
                C_ = Matrix<T>(A_.GetRowsNb(), A_.GetColsNb());
                MatMult(C_, MatMult(V_, S_), MatHermitian(V_));
            }
            return C_;
        }

        inline int GetFlags() const { return flags_; }

        inline void SetFlags(int flags) { flags_ = flags; }

        inline const Matrix<T>& Compute(const int& flags = 0)
        {
            flags_        = flags;
            Matrix<T>* pV = nullptr;
            if (flags_ & SCHUR::COMPUTE_V) pV = &V_;
            MatSchur(E_, pV, S_, A_, flags_);
            return E_;
        }

      private:
        const Matrix<T>& A_;
        Matrix<T> E_;
        Matrix<T> S_;
        Matrix<T> V_;
        Matrix<T> C_;
        int flags_;
    };

} // namespace la

#endif
