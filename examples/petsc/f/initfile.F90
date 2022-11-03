!****************************************************************
!****
!**** This program file is part of the tutorial
!**** `Introduction to the PETSc library'
!**** by Victor Eijkhout eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2020
!****
!**** init.F90 : petsc initialization
!****
!****************************************************************

Program PetscInit

#include <petsc/finclude/petscsys.h>
  use petsc
  implicit none
  
  logical :: flag
  PetscErrorCode ierr;
  call PetscInitialize("inithelp",ierr)
  CHKERRA(ierr)
  call MPI_Initialized(flag,ierr)
  CHKERRA(ierr)
  if (flag) then
     print *,"MPI was initialized by PETSc"
  else
     print *,"MPI not yet initialized"
  end if
  call PetscFinalize(ierr); CHKERRQ(ierr);

End Program PetscInit
