!/***********************/
!/*      main.f90       */
!/*    Version 1.0      */
!/*    2022/06/05       */
!/***********************/

PROGRAM MAIN

    INTEGER IDIM, IDIM1

    IDIM = 35
    IDIM1= 45

    WRITE(6,*) 'Inside Fortran calling first C function'
    CALL CFUN(IDIM)
    WRITE(6,*) 'Inside Fortran calling second C function'
    CALL CFUN1(IDIM1)
    WRITE(6,*) 'Exiting the Fortran program'

END PROGRAM MAIN
