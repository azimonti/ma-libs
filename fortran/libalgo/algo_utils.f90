!/************************/
!/*  algo_utils.F90      */
!/*    VERSION 1.0       */
!/************************/

MODULE ALGO_UTILS
    IMPLICIT NONE

! PRIVATE ::
PUBLIC  :: QUICKSORT_I4, QUICKSORT_R8

CONTAINS

    RECURSIVE SUBROUTINE QUICKSORT_I4(V, FIRST, LAST)
        !DEC$ ATTRIBUTES DLLEXPORT :: QUICKSORT_I4
        INTEGER, DIMENSION(:), INTENT(INOUT) :: V
        INTEGER , INTENT(IN)                 :: FIRST, LAST
        INTEGER                              :: X, T, I, J

        X = V( (FIRST+LAST) / 2 )
        I = FIRST
        J = LAST
        DO
            DO WHILE (V(I) < X)
                I=I+1
            END DO
            DO WHILE (X < V(J))
                J=J-1
            END DO
            IF (I >= J) EXIT
            T = V(I);  V(I) = V(J);  V(J) = T
            I=I+1
            J=J-1
        END DO
        IF (FIRST < I-1) CALL QUICKSORT_I4(V, FIRST, I-1)
        IF (J+1 < LAST)  CALL QUICKSORT_I4(V, J+1, LAST)
    END SUBROUTINE QUICKSORT_I4

    RECURSIVE SUBROUTINE QUICKSORT_R8(V, FIRST, LAST)
        !DEC$ ATTRIBUTES DLLEXPORT :: QUICKSORT_R8
        INTEGER, PARAMETER                       :: KINDR = KIND(0D0)
        REAL(KINDR), DIMENSION(:), INTENT(INOUT) :: V
        INTEGER , INTENT(IN)                     :: FIRST, LAST
        REAL(KINDR)                              :: X, T
        INTEGER                                  :: I, J

        X = V( (FIRST+LAST) / 2 )
        I = FIRST
        J = LAST
        DO
            DO WHILE (V(I) < X)
                I=I+1
            END DO
            DO WHILE (X < V(J))
                J=J-1
            END DO
            IF (I >= J) EXIT
            T = V(I);  V(I) = V(J);  V(J) = T
            I=I+1
            J=J-1
        END DO
        IF (FIRST < I-1) CALL QUICKSORT_R8(V, FIRST, I-1)
        IF (J+1 < LAST)  CALL QUICKSORT_R8(V, J+1, LAST)
    END SUBROUTINE QUICKSORT_R8

END MODULE ALGO_UTILS
