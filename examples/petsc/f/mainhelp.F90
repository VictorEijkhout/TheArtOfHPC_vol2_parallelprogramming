! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2022
!****
!**** mainhelp : Petsc example for help info
!****
!****************************************************************/

Program mainhelp
#include <petsc/finclude/petsc.h>
  use petsc
  implicit none
  PetscErrorCode :: ierr

  Character(len=50) :: help = "This program demonstrates help info"
  help = trim(help) // NEW_LINE('A')
  call PetscInitialize(PETSC_NULL_CHARACTER,help,ierr)
  CHKERRA(ierr)
  call PetscFinalize(ierr)
  CHKERRA(ierr)

end Program mainhelp
