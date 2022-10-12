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
  CHKERRA(ierr)
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
  CHKERRA(ierr);
  Nglobal = Nglobal * x

  call VecCreate(comm,in,ierr)
  CHKERRA(ierr);
  call VecSetType(in,VECMPI,ierr)
  CHKERRA(ierr);
  call VecSetSizes(in,PETSC_DECIDE,Nglobal,ierr)
  CHKERRA(ierr);
  call VecDuplicate(in,out,ierr)
  CHKERRA(ierr);
  
  call VecGetOwnershipRange(in,myfirst,mylast,ierr)
  CHKERRA(ierr);
  do index=myfirst,mylast-1
     v = index
     call VecSetValue(in,index,v,INSERT_VALUES,ierr)
     CHKERRA(ierr);
  end do
  call VecAssemblyBegin(in,ierr)
  CHKERRA(ierr);
  call VecAssemblyEnd(in,ierr)
  CHKERRA(ierr);

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
     CHKERRA(ierr);
  else
     !call ISCreateStride(comm,Nglobal/2,1,2,oddeven,ierr)
!!!! your code here !!!!
     CHKERRA(ierr);
  end if
  call ISView(oddeven,PETSC_VIEWER_STDOUT_WORLD,ierr);
  CHKERRA(ierr)

  call VecScatterCreate(in,oddeven,out,PETSC_NULL_IS,separate,ierr)
  CHKERRA(ierr);
  call VecScatterBegin(separate,in,out,INSERT_VALUES,SCATTER_FORWARD,ierr)
  CHKERRA(ierr);
  call VecScatterEnd(separate,in,out,INSERT_VALUES,SCATTER_FORWARD,ierr)
  CHKERRA(ierr);

  call ISDestroy(oddeven,ierr)
  CHKERRA(ierr);
  call VecScatterDestroy(separate,ierr)
  CHKERRA(ierr);

  call VecView(in,PETSC_VIEWER_STDOUT_WORLD,ierr)
  CHKERRA(ierr);
  call VecView(out,PETSC_VIEWER_STDOUT_WORLD,ierr)
  CHKERRA(ierr);

  call VecDestroy(in,ierr)
  CHKERRA(ierr);
  call VecDestroy(out,ierr)
  CHKERRA(ierr);

  call PetscFinalize(ierr)
  CHKERRA(ierr);

end Program OddEvenSplit
