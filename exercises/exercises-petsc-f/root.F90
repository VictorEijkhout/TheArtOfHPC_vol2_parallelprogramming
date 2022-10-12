!****************************************************************
!****
!**** This program file is part of the tutorial
!**** `Introduction to the PETSc library'
!**** by Victor Eijkhout eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2020
!****
!**** backtrace.F90 : demonstrate backtracing error messages
!****
!****************************************************************

Program ComputeRoot

#include <petsc/finclude/petsc.h>
  use petsc
  implicit none

  PetscErrorCode :: ierr
  PetscReal :: x,rootx

  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRA(ierr)

  !
  ! Compute the square root of a positive number
  !
  x = 1.5
  call SquareRoot(x,rootx,ierr)
  CHKERRA(ierr)
  print *,"The root of",x,"is",rootx
  !
  ! Fail to compute the square root of a negative number
  !
  x = -2.6
  call SquareRoot(x,rootx,ierr)
  CHKERRA(ierr)
  print *,"The root of",x,"is",rootx

  call PetscFinalize(ierr)
  CHKERRA(ierr)

contains

  Subroutine SquareRoot(x,rootx,ierr)
    implicit none
    PetscReal,intent(in) :: x
    PetscReal,intent(out) :: rootx
    integer,intent(out) :: ierr
    character*80 :: msg

!!!! your code here !!!!

  end Subroutine SquareRoot

End Program ComputeRoot
