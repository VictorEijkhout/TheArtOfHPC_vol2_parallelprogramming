!--------------------------------------------
! Example code for debugging training at TACC
!
! v1.0, 2015-06-24, carlos@tacc.utexas.edu
!--------------------------------------------
PROGRAM main

INTEGER :: myDiv
INTEGER :: res, x = 5, y

DO y = 1, 10
  res = myDiv(x,y)
  WRITE(*,*) x,y,res
END DO


END PROGRAM


FUNCTION myDiv(x,y)
  INTEGER, INTENT(IN) :: x, y
  myDiv = 1/(x-y)
  RETURN
END FUNCTION myDiv
