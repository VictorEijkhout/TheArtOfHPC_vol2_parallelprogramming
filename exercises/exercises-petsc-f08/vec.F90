! -*- f90 -*-
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!
!!!! This program file is part of the tutorial
!!!! `Introduction to the PETSc library'
!!!! by Victor Eijkhout eijkhout@tacc.utexas.edu
!!!!
!!!! copyright Victor Eijkhout 2012-2021
!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program Vector
#include <petsc/finclude/petsc.h>
  use petsc
  implicit none
  
  MPI_Comm       :: comm
  Vec            :: x,y
  PetscInt       :: n=200, procno
  PetscScalar    :: one=1.0, two=2.0, value, inprod,scaling,norm
  PetscInt       :: globalsize,localsize,myfirst,mylast,index
  PetscBool      :: flag
  PetscErrorCode :: ierr
  
  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRQ(ierr)
  comm = PETSC_COMM_WORLD
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Get a commandline argument for the size of the problem
  !!
  call PetscOptionsGetInt( &
       PETSC_NULL_OPTIONS,PETSC_NULL_CHARACTER, &
       "-n",n,flag,ierr)
  CHKERRQ(ierr)

  !!
  !! Create vector `x' with a default layout
  !!
  call VecCreate(comm,x,ierr); CHKERRQ(ierr)
  call VecSetSizes(x,PETSC_DECIDE,n,ierr); CHKERRQ(ierr)
  call VecSetFromOptions(x,ierr); CHKERRQ(ierr)

  !!
  !! Make another vector, just like x
  !!
  call VecDuplicate(x,y,ierr); CHKERRQ(ierr)

  !!
  !! Set x,y to constant values
  !!
  call VecSet(x,one,ierr); CHKERRQ(ierr)
  !!
  !! Exercise 1:
  !! -- Set y to a sine wave
  !!    Find the correct bounds on the loop
  !!    and the correct index,
  !!    so that each process only sets local values.
  !!
  call VecGetSize(y,globalsize,ierr); CHKERRQ(ierr)
  call VecGetLocalSize(y,localsize,ierr); CHKERRQ(ierr)
  call VecGetOwnershipRange(y,myfirst,mylast,ierr)
  CHKERRQ(ierr)
  do index= &
!!!! your code here !!!!
     value = sin( index * 2 * 3.14159 / globalsize )
     call VecSetValue(y,&
!!!! your code here !!!!
          value,INSERT_VALUES,ierr); CHKERRQ(ierr)
  end do
  call VecAssemblyBegin(y,ierr); CHKERRQ(ierr)
  call VecAssemblyEnd(y,ierr); CHKERRQ(ierr)

  !!
  !! Exercise 2:
  !! -- Compute inner product of x,y
  !!
!!!! your code here !!!!
  if (procno==0) &
       print *,"Inner product computed as",inprod,"; should be aprox zero"

  !!
  !! Exercise 3:
  !! -- compute the norm of x
  !! -- scale x down by that norm
  !! -- check that the norm of the result is 1
  !!
!!!! your code here !!!!
  if (procno==0) &
       print *,"Norm of scaled vector is",norm,"; should be 1"

  !!
  !! Free work space. All PETSc objects should be destroyed when they
  !! are no longer needed
  !!
  call VecDestroy(x,ierr)
  call VecDestroy(y,ierr)

  call PetscFinalize(ierr);
  CHKERRQ(ierr)
  
END program Vector
