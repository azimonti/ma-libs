#ifndef _LA_BLAS_MULT_H_D7DFF0D5BDE7488692867085271F29EE_
#define _LA_BLAS_MULT_H_D7DFF0D5BDE7488692867085271F29EE_

/************************/
/*   la_blas_mult.h     */
/*     Version 1.0      */
/*     2023/06/06       */
/************************/

#ifndef USE_BLAS
#error "USE_BLAS is not defined"
#endif

#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"

namespace la
{

    template <typename T> Matrix<T>& MatMultVec(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B);

    template <typename T> Matrix<T>& MatMult(Matrix<T>& res, const Matrix<T>& A, const Matrix<T>& B);

    template <typename T> inline Matrix<T> MatMultVec(const Matrix<T>& A, const Matrix<T>& B)
    {
        Matrix<T> res_{A.GetRowsNb(), 1};
        return MatMultVec(res_, A, B);
    }

    template <typename T> inline Matrix<T> MatMult(const Matrix<T>& A, const Matrix<T>& B)
    {
        Matrix<T> res_{A.GetRowsNb(), B.GetColsNb()};
        return la::MatMult(res_, A, B);
    }

} // namespace la

#endif
