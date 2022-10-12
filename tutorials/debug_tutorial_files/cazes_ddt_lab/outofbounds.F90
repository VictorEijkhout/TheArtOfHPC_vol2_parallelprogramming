!--------------------------------------------
! Example code for debugging training at TACC
!
! v1.0, 2015-06-24, carlos@tacc.utexas.edu
!--------------------------------------------
PROGRAM main

INTEGER :: i
REAL*8, ALLOCATABLE, DIMENSION(:) :: a, b

ALLOCATE( a(1:5) )
ALLOCATE( b(1:5) )

a(:) = 1
b(:) = 2

DO i = 0, 4
  a(i) = 2*b(i)
  WRITE(*,*) 'a(',i,')=',a(i)
END DO

DEALLOCATE( a )
DEALLOCATE( b )

END PROGRAM
