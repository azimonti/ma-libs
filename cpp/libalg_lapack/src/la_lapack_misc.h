#ifndef _LA_LAPACK_MISC_H_6FBE38C07EB2457BAC2AEB3623C6DD5A_
#define _LA_LAPACK_MISC_H_6FBE38C07EB2457BAC2AEB3623C6DD5A_

/************************/
/*   la_lapack_misc.h   */
/*    Version 1.0       */
/*     2023/06/13       */
/************************/

#ifndef USE_LAPACK
#error "USE_LAPACK is not defined"
#endif

#include "la_blas_mult.h"
#include "math/algebra/matrix.h"
#include "math/algebra/matrix_operations.h"
#include "math/algebra/tolerance.h"

namespace la
{

    namespace LAPACK_MISC
    {
        enum Flags : int {
            // any flag for future use
            // FLAG1 = 1 << 0,
        };
    }

    template <typename T> size_t MatRank(const Matrix<T>& A, const T& tol = DefaultTol<T>());
    template <typename T> T MatDet(const Matrix<T>& A);

} // namespace la

#endif
