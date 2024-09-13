!/************************/
!/*   routines01.F90     */
!/*    VERSION 1.0       */
!/*     2022/09/07       */
!/*  © MARCO AZIMONTI    */
!/************************/

MODULE TEST_ROUTINES
IMPLICIT NONE

! PRIVATE ::
PUBLIC  :: CHECK_LOGICAL, CHECK_I4, CHECK_I8, CHECK_R8

CONTAINS

    SUBROUTINE CHECK_LOGICAL(L1, TOTAL_CHECK, TOTAL_ERROR)
        !DEC$ ATTRIBUTES DLLEXPORT :: CHECK_LOGICAL
        LOGICAL, INTENT(IN)              :: L1
        INTEGER, INTENT(INOUT)           :: TOTAL_CHECK
        INTEGER, INTENT(INOUT), OPTIONAL :: TOTAL_ERROR

        TOTAL_CHECK = TOTAL_CHECK + 1
        IF (.NOT. L1) THEN
            IF(PRESENT(TOTAL_ERROR)) THEN
                TOTAL_ERROR = TOTAL_ERROR + 1
            ENDIF
            WRITE(*,'(A, L1, A)') "CHECK_LOGICAL: ", L1, &
      &           " IS FALSE."
        ENDIF
        RETURN
    END SUBROUTINE CHECK_LOGICAL


    SUBROUTINE CHECK_I4(I1, I2, TOTAL_CHECK, TOTAL_ERROR)
        !DEC$ ATTRIBUTES DLLEXPORT :: CHECK_I4
        INTEGER, INTENT(IN)              :: I1, I2
        INTEGER, INTENT(INOUT)           :: TOTAL_CHECK
        INTEGER, INTENT(INOUT), OPTIONAL :: TOTAL_ERROR

        TOTAL_CHECK = TOTAL_CHECK + 1
        IF (I1 .NE. I2) THEN
            IF(PRESENT(TOTAL_ERROR)) THEN
                TOTAL_ERROR = TOTAL_ERROR + 1
            ENDIF
            WRITE(*,'(A, I0, A, I0, A)') "CHECK_I4: ", I1, " AND ", I2, &
      &           " ARE NOT EQUAL."
        ENDIF
        RETURN
    END SUBROUTINE CHECK_I4


    SUBROUTINE CHECK_I8(I1, I2, TOTAL_CHECK, TOTAL_ERROR)
        !DEC$ ATTRIBUTES DLLEXPORT :: CHECK_I8
        INTEGER (KIND=8), INTENT(IN)     :: I1, I2
        INTEGER, INTENT(INOUT)           :: TOTAL_CHECK
        INTEGER, INTENT(INOUT), OPTIONAL :: TOTAL_ERROR

        TOTAL_CHECK = TOTAL_CHECK + 1
        IF (I1 .NE. I2) THEN
            IF(PRESENT(TOTAL_ERROR)) THEN
                TOTAL_ERROR = TOTAL_ERROR + 1
            ENDIF
            WRITE(*,'(A, I0, A, I0, A)') "CHECK_I8: ", I1, " AND ", I2, &
      &         " ARE NOT EQUAL."
        ENDIF
        RETURN
    END SUBROUTINE CHECK_I8


    SUBROUTINE CHECK_R8(R1, R2, TOTAL_CHECK, TOTAL_ERROR)
        !DEC$ ATTRIBUTES DLLEXPORT :: CHECK_R8
        REAL (KIND=8), INTENT(IN)        :: R1, R2
        INTEGER, INTENT(INOUT)           :: TOTAL_CHECK
        INTEGER, INTENT(INOUT), OPTIONAL :: TOTAL_ERROR

        TOTAL_CHECK = TOTAL_CHECK + 1
        IF (ABS(R1 - R2) .GE. 0.0000001) THEN
            IF(PRESENT(TOTAL_ERROR)) THEN
                TOTAL_ERROR = TOTAL_ERROR + 1
            ENDIF
            WRITE(*,'(A, F5.2, A, F5.2, A)') "CHECK_I8: ", R1, " AND ", &
      &          R2, " ARE NOT EQUAL."
        ENDIF
        RETURN
    END SUBROUTINE CHECK_R8

END MODULE TEST_ROUTINES
