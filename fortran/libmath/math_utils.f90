!/************************/
!/*  math_utils.F90      */
!/*    VERSION 1.0       */
!/*     2022/10/14       */
!/************************/

MODULE MATH_UTILS
    IMPLICIT NONE

! PRIVATE ::
PUBLIC  :: TESTMATH_I4

CONTAINS
    SUBROUTINE TESTMATH_I4(I)
        !DEC$ ATTRIBUTES DLLEXPORT :: TESTMATH_I4
        INTEGER, INTENT(INOUT) :: I
        I = 1
    END SUBROUTINE TESTMATH_I4

END MODULE MATH_UTILS
