!/***************************/
!/*  external_function.f90  */
!/*    Version 1.0          */
!/*     2022/06/05          */
!/***************************/

REAL(8) FUNCTION ADD (A, B, C, D)
    IMPLICIT NONE

    REAL(8) B,D
    INTEGER(4) A,C
    DIMENSION B(4), D(4)
    ADD = B(A) + D(C)
    RETURN
END

SUBROUTINE EXEC(ROUTINE)
    IMPLICIT NONE

    EXTERNAL         :: ROUTINE
    REAL(KIND(1.D0)) :: RES

    CALL ROUTINE(3.0D0, RES)
    PRINT *, "Fortran Result: res = ", RES

END SUBROUTINE EXEC
