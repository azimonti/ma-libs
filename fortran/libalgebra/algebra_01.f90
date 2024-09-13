!/***********************/
!/*   algebra01.f90     */
!/*    VERSION 1.0      */
!/*    2023/03/18       */
!/***********************/

MODULE ALGEBRA_01
  IMPLICIT NONE

! PRIVATE ::
PUBLIC  :: RREF

CONTAINS
    SUBROUTINE RREF(MATRIX, TOLERANCE)
        !DEC$ ATTRIBUTES DLLEXPORT :: RREF
        INTEGER, PARAMETER                         :: KINDR = KIND(0D0)
        REAL(KINDR), DIMENSION(:,:), INTENT(INOUT) :: MATRIX
        REAL(KINDR), OPTIONAL,       INTENT(IN)    :: TOLERANCE
        REAL(KINDR)                                :: TOL

        INTEGER :: PIVOT, NROWS, NCOLS
        INTEGER :: C, I
        REAL(KINDR), DIMENSION(:), ALLOCATABLE :: TROW

        IF(PRESENT(TOLERANCE)) THEN
            TOL = TOLERANCE
        ELSE
            TOL = 1E-9
        END IF

        PIVOT = 1
        NROWS = SIZE(MATRIX, 1)
        NCOLS = SIZE(MATRIX, 2)

        ALLOCATE(TROW(NCOLS))

        DO C = 1, NROWS
            IF (MIN(NCOLS, NROWS) < PIVOT) EXIT
            I = C
            DO WHILE (ABS(MATRIX(I, PIVOT)) < TOL)
                I = I + 1
                IF (NROWS == I) THEN
                    I = C
                    PIVOT = PIVOT + 1
                    IF (NCOLS == PIVOT) THEN
                        DEALLOCATE(TROW)
                        RETURN
                    END IF
                END IF
            END DO
            TROW = MATRIX(I, :)
            MATRIX(I, :) = MATRIX(C, :)
            MATRIX(C, :) = TROW
            MATRIX(C, :) = MATRIX(C, :) / MATRIX(C, PIVOT)
            DO I = 1, NROWS
                IF (I .NE. C ) MATRIX(I, :) = MATRIX(I, :) - MATRIX(C, :) * MATRIX(I, PIVOT)
            END DO
            PIVOT = PIVOT + 1
        END DO
        DEALLOCATE(TROW)
    END SUBROUTINE RREF

END MODULE ALGEBRA_01
