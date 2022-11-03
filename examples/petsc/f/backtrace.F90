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

Program BackTrace

#include <petsc/finclude/petsc.h>
  use petsc
  implicit none

  PetscErrorCode :: ierr

  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRA(ierr)
  call this_function_bombs(ierr)
  CHKERRA(ierr)
  call PetscFinalize(ierr)
  CHKERRA(ierr)

contains

  Subroutine this_function_bombs(ierr)
    implicit none
    integer,intent(out) :: ierr

    SETERRQ(PETSC_COMM_SELF,1,"We cannot go on like this")
    ierr = -1

  end Subroutine this_function_bombs

End Program BackTrace
