!****************************************************************
!****
!**** This program file is part of the tutorial
!**** `Introduction to the PETSc library'
!**** by Victor Eijkhout eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2022
!****
!**** backtrace.F90 : program with runtime error
!****
!****************************************************************

Program PetscInit

#include <petsc/finclude/petscsys.h>
  use petsc
  implicit none
  
  PetscErrorCode ierr

  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRA(ierr)

  call this_function_bombs()

  call PetscFinalize(ierr); CHKERRQ(ierr);

contains
  subroutine this_function_bombs()
    PetscErrorCode :: code=1
    character*80 :: msg="We can not go on like this"
    SETERRQ(PETSC_COMM_SELF,code,msg)
  end subroutine this_function_bombs
End Program PetscInit
