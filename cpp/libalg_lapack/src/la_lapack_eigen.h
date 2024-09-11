#ifndef _LA_LAPACK_EIGEN_H_AB5E309642E948F29E48E75020050468_
#define _LA_LAPACK_EIGEN_H_AB5E309642E948F29E48E75020050468_

/************************/
/*  la_lapack_eigen.h   */
/*    Version 1.0       */
/*     2023/06/11       */
/************************/

#ifndef USE_LAPACK
#error "USE_LAPACK is not defined"
#endif

#include <complex>
#include <limits>
#include <type_traits>
#include <typeinfo>
#include "la_blas_mult.h"
#include "la_lapack_macro.h"
#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"

namespace la
{

    namespace EIGEN
    {
        enum Flags : int {
            COMPUTE_VL = 1 << 0,
            COMPUTE_VR = 1 << 1,
        };
    }

    template <typename T>
    Matrix<T>& MatEigen(Matrix<T>& E, Matrix<T>* pVL, Matrix<T>* pVR, const Matrix<T>& A, const int& flags = 0);

    template <typename T> Matrix<T>& MatEigen(Matrix<T>& E, Matrix<T>* pVR, const Matrix<T>& A, const int& flags = 0)
    {
        return MatEigen<T>(E, nullptr, pVR, A, flags);
    }

    template <typename T> Matrix<T>& MatEigen(Matrix<T>& E, const Matrix<T>& A, const int& flags = 0)
    {
        return MatEigen<T>(E, nullptr, nullptr, A, flags);
    }

    template <typename T> class MatrixEigen
    {

        REALTYPE_DEFINE
      public:
        inline MatrixEigen(const Matrix<T>& A)
            : A_(A), VL_(0, 0), VR_(0, 0), AC_(0, 0), EC_(0, 0), VLC_(0, 0), VRC_(0, 0), AV_(0, 0), EV_(0, 0), flags_(0)
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
            else { throw std::invalid_argument("MatrixEigen: invalid type"); }
        }

        inline const Matrix<T>& A() const { return A_; }

        inline const Matrix<T>& E() const { return E_; }

        inline const Matrix<std::complex<RealType>>& EC()
        {
            if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
            {
                if (EC_.size() == 0)
                {
                    EC_ = Matrix<std::complex<RealType>>(E_.GetRowsNb(), 1);
                    for (size_t i = 0; i < E_.GetRowsNb(); i++) EC_(i, 0) = std::complex<RealType>(E_(i, 0), E_(i, 1));
                }
                return EC_;
            }
            // for complex matrices, EC_ is equal to E_
            else return E_;
        }

        inline const Matrix<T>& VL() const { return VL_; }

        inline const Matrix<std::complex<RealType>>& VLC()
        {
            if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
            {
                if (VLC_.size() == 0)
                {
                    VLC_ = Matrix<std::complex<RealType>>(VL_.GetRowsNb(), VL_.GetColsNb());
                    for (size_t i = 0; i < VL_.GetRowsNb(); i++)
                        for (size_t j = 0; j < VL_.GetColsNb(); j++)
                        {
                            if (j < VL_.GetColsNb() - 1 &&
                                std::abs(VL_(i, j + 1)) > std::numeric_limits<RealType>::epsilon())
                            {
                                // Complex conjugate pair
                                VLC_(i, j)     = std::complex<RealType>(VL_(i, j), VL_(i, j + 1));
                                VLC_(i, j + 1) = std::complex<RealType>(VL_(i, j), -VL_(i, j + 1));
                                j++; // Skip the next column since it represents the complex conjugate
                            }
                            else
                            {
                                // Real eigenvalue
                                VLC_(i, j) = std::complex<RealType>(VL_(i, j), 0.0);
                            }
                        }
                }
                return VLC_;
            }
            // For complex matrices, VLC_ is equal to VL_
            else return VL_;
        }

        inline const Matrix<T>& VR() const { return VR_; }

        inline const Matrix<std::complex<RealType>>& VRC()
        {
            if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
            {
                if (VRC_.size() == 0)
                {
                    VRC_ = Matrix<std::complex<RealType>>(VR_.GetRowsNb(), VR_.GetColsNb());
                    for (size_t i = 0; i < VR_.GetRowsNb(); i++)
                    {
                        for (size_t j = 0; j < VR_.GetColsNb(); j++)
                        {
                            if (j < VR_.GetColsNb() - 1 &&
                                std::abs(VR_(i, j + 1)) > std::numeric_limits<RealType>::epsilon())
                            {
                                // Complex conjugate pair
                                VRC_(i, j)     = std::complex<RealType>(VR_(i, j), VR_(i, j + 1));
                                VRC_(i, j + 1) = std::complex<RealType>(VR_(i, j), -VR_(i, j + 1));
                                j++; // Skip the next column since it represents the complex conjugate
                            }
                            else
                            {
                                // Real eigenvalue
                                VRC_(i, j) = std::complex<RealType>(VR_(i, j), 0.0);
                            }
                        }
                    }
                }
                return VRC_;
            }
            // For complex matrices, VRC_ is equal to VR_
            else return VR_;
        }

        inline const Matrix<std::complex<RealType>> AV(const bool& right = true)
        {
            if (AC_.size() == 0)
            {
                if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
                {

                    AC_ = Matrix<std::complex<RealType>>(A_.GetRowsNb(), A_.GetColsNb());
                    // copy A_ into AC_
                    for (size_t i = 0; i < A_.GetRowsNb(); i++)
                        for (size_t j = 0; j < A_.GetColsNb(); j++) AC_(i, j) = A_(i, j);
                }
                else AC_ = A_;
            }
            if (AV_.size() == 0) AV_ = Matrix<std::complex<RealType>>(A_.GetRowsNb(), A_.GetColsNb());
            if (right) MatMult(AV_, AC_, VRC());
            else MatMult(AV_, MatHermitian(VLC()), AC_);
            return AV_;
        }

        inline const Matrix<std::complex<RealType>> EV(const bool& right = true)
        {

            if (EV_.size() == 0) EV_ = Matrix<std::complex<RealType>>(A_.GetRowsNb(), A_.GetColsNb());
            // create a diagonal matrix with eigenvalues
            Matrix<std::complex<RealType>> D(A_.GetRowsNb(), A_.GetColsNb());
            for (size_t i = 0; i < A_.GetRowsNb(); i++) D(i, i) = EC()(i, 0);
            if (right) MatMult(EV_, D, VRC());
            else MatMult(EV_, D, MatHermitian(VLC()));
            return AV_;
        }

        inline int GetFlags() const { return flags_; }

        inline void SetFlags(int flags) { flags_ = flags; }

        inline const Matrix<T>& Compute(const int& flags = 0)
        {
            flags_        = flags;
            Matrix<T>*pVL = nullptr, *pVR = nullptr;
            if (flags_ & EIGEN::COMPUTE_VL)
            {
                if (VL_.size() == 0) VL_ = Matrix<T>(A_.GetRowsNb(), A_.GetColsNb());
                pVL = &VL_;
            }
            if (flags_ & EIGEN::COMPUTE_VR)
            {
                if (VR_.size() == 0) VR_ = Matrix<T>(A_.GetRowsNb(), A_.GetColsNb());
                pVR = &VR_;
            }
            MatEigen<T>(E_, pVL, pVR, A_, flags_);
            return E_;
        }

      private:
        const Matrix<T>& A_;
        Matrix<T> E_;
        Matrix<T> VL_;
        Matrix<T> VR_;
        Matrix<std::complex<RealType>> AC_;
        Matrix<std::complex<RealType>> EC_;
        Matrix<std::complex<RealType>> VLC_;
        Matrix<std::complex<RealType>> VRC_;
        Matrix<std::complex<RealType>> AV_;
        Matrix<std::complex<RealType>> EV_;

        int flags_;
    };

} // namespace la

#endif
