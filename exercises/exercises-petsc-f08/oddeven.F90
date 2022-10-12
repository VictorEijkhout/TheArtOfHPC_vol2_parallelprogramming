!****************************************************************
!****
!**** This program file is part of the tutorial
!**** `Introduction to the PETSc library'
!**** by Victor Eijkhout eijkhout@tacc.utexas.edu
!**** copyright Victor Eijkhout 2012-2020
!****
!**** 
!**** oddeven.c : exercise for splitting a vector by IS and VecScatter
!****
!****************************************************************

Program OddEvenSplit
  
#include "petsc/finclude/petsc.h"
  use petsc
  implicit none

  PetscErrorCode :: ierr
  MPI_Comm :: comm
  integer :: nprocs,procid
  PetscInt :: Nglobal,x=1
  PetscInt :: myfirst,mylast,index
  Vec :: in,out
  PetscScalar :: v
  IS :: oddeven
  VecScatter :: separate

  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRQ(ierr)
  comm = MPI_COMM_WORLD;

  call MPI_Comm_rank(comm,procid,ierr)
  call MPI_Comm_size(comm,nprocs,ierr)
  if (nprocs/=2) then 
     call PetscPrintf(comm,"This example only works on 2 processes\n",ierr)
     call MPI_Abort(comm,-1,ierr)
  end if

  Nglobal = 2*nprocs
  call PetscOptionsGetInt &
       (PETSC_NULL_OPTIONS,PETSC_NULL_CHARACTER,"-x",x,PETSC_NULL_BOOL,ierr)
  CHKERRQ(ierr);
  Nglobal = Nglobal * x

  call VecCreate(comm,in,ierr)
  CHKERRQ(ierr);
  call VecSetType(in,VECMPI,ierr)
  CHKERRQ(ierr);
  call VecSetSizes(in,PETSC_DECIDE,Nglobal,ierr)
  CHKERRQ(ierr);
  call VecDuplicate(in,out,ierr)
  CHKERRQ(ierr);
  
  call VecGetOwnershipRange(in,myfirst,mylast,ierr)
  CHKERRQ(ierr);
  do index=myfirst,mylast-1
     v = index
     call VecSetValue(in,index,v,INSERT_VALUES,ierr)
     CHKERRQ(ierr);
  end do
  call VecAssemblyBegin(in,ierr)
  CHKERRQ(ierr);
  call VecAssemblyEnd(in,ierr)
  CHKERRQ(ierr);

   !!
   !! Exercise:
   !! -- code is given here for splitting a vector into odd/even indices
   !! -- change the IS objects so that indices will additionally be
   !!    ordered from high to low index
   !!
  if (procid==0) then
     !!
     !! here is code for the original ordering, first enable this,
     !! then figure out how to reverse it.
     !!
     !call ISCreateStride(comm,Nglobal/2,0,2,oddeven,ierr)
!!!! your code here !!!!
     CHKERRQ(ierr);
  else
     !call ISCreateStride(comm,Nglobal/2,1,2,oddeven,ierr)
!!!! your code here !!!!
     CHKERRQ(ierr);
  end if
  call ISView(oddeven,PETSC_VIEWER_STDOUT_WORLD,ierr);
  CHKERRQ(ierr)

  call VecScatterCreate(in,oddeven,out,PETSC_NULL_IS,separate,ierr)
  CHKERRQ(ierr);
  call VecScatterBegin(separate,in,out,INSERT_VALUES,SCATTER_FORWARD,ierr)
  CHKERRQ(ierr);
  call VecScatterEnd(separate,in,out,INSERT_VALUES,SCATTER_FORWARD,ierr)
  CHKERRQ(ierr);

  call ISDestroy(oddeven,ierr)
  CHKERRQ(ierr);
  call VecScatterDestroy(separate,ierr)
  CHKERRQ(ierr);

  call VecView(in,PETSC_VIEWER_STDOUT_WORLD,ierr)
  CHKERRQ(ierr);
  call VecView(out,PETSC_VIEWER_STDOUT_WORLD,ierr)
  CHKERRQ(ierr);

  call VecDestroy(in,ierr)
  CHKERRQ(ierr);
  call VecDestroy(out,ierr)
  CHKERRQ(ierr);

  call PetscFinalize(ierr)
  CHKERRQ(ierr);

end Program OddEvenSplit
