!****************************************************************
!****
!**** This program file is part of the tutorial
!**** `Introduction to the PETSc library'
!**** by Victor Eijkhout eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2020
!****
!**** vecset.F90 : petsc initialization
!****
!****************************************************************

Program VecSetF90

#include <petsc/finclude/petsc.h>
  use petsc
  implicit none

  Vec            :: vector
  PetscScalar,dimension(:),pointer :: elements
  PetscErrorCode :: ierr
  PetscInt       :: globalsize
  integer        :: myrank,vecidx,comm
  PetscScalar    :: vecelt
  character*80   :: msg  

  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRA(ierr)
  comm = MPI_COMM_WORLD
  call MPI_Comm_rank(comm,myrank,ierr)

  call VecCreate(comm,vector,ierr)
  call VecSetType(vector,VECMPI,ierr)
  call VecSetSizes(vector,2,PETSC_DECIDE,ierr)
  call VecGetSize(vector,globalsize,ierr)

  if (myrank==0) then
     do vecidx=0,globalsize-1
        vecelt = vecidx
        call VecSetValue(vector,vecidx,vecelt,INSERT_VALUES,ierr)
     end do
  end if
  call VecAssemblyBegin(vector,ierr)
  call VecAssemblyEnd(vector,ierr)
  call VecView(vector,PETSC_VIEWER_STDOUT_WORLD,ierr)

  call VecGetArrayF90(vector,elements,ierr)
  write (msg,10) myrank,elements(1)
10 format("First element on process",i3,":",f7.4,"\n")
  call PetscSynchronizedPrintf(comm,msg,ierr)
  call PetscSynchronizedFlush(comm,PETSC_STDOUT,ierr)
  call VecRestoreArrayF90(vector,elements,ierr)

  call VecDestroy(vector,ierr)
  call PetscFinalize(ierr); CHKERRQ(ierr);

End Program VecSetF90
