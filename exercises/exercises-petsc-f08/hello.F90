! -*- f90 -*-
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!
!!!! This program file is part of the tutorial
!!!! `Introduction to the PETSc library'
!!!! by Victor Eijkhout eijkhout@tacc.utexas.edu
!!!!
!!!! copyright Victor Eijkhout 2012-9
!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program Hello
#include <petsc/finclude/petsc.h>
  use petsc
  implicit none
  
  character*80 msg
  PetscInt       :: mytid,ntids
  PetscErrorCode :: ierr
  MPI_Comm       :: comm
  
  call PetscInitialize(PETSC_NULL_CHARACTER,ierr);  CHKERRQ(ierr)
  comm = PETSC_COMM_WORLD
  
  call MPI_Comm_rank(MPI_COMM_WORLD,mytid,ierr);  CHKERRQ(ierr)
  call MPI_Comm_size(MPI_COMM_WORLD,ntids,ierr);  CHKERRQ(ierr)

  !!
  !! Exercise 1:
  !! -- use PetscPrintf to print a message only from process zero
  !!    make sure to remove the original write statement!
  !!
10 format('Hello, I am processor',i3,' out of',i3,'\n')
  write(*,10) mytid,ntids
  write(msg,10) mytid,ntids
  call PetscPrintf( &
!!!! your code here !!!!
       );  CHKERRQ(ierr)
  
  !!
  !! Exercise 2:
  !! -- use PetscSynchronizedPrintf to let each processor print in sequence
  !!
#if 0
  call PetscSynchronizedPrintf( &
!!!! your code here !!!!
       )
  call PetscSynchronizedFlush(comm,0,ierr)
#endif

  call PetscFinalize(ierr);  CHKERRQ(ierr)
  
END program Hello
