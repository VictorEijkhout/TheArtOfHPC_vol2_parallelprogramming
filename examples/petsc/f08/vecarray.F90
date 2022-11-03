! -*- f90 -*-
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!
!!!! This program file is part of the tutorial
!!!! `Introduction to the PETSc library'
!!!! by Victor Eijkhout eijkhout@tacc.utexas.edu
!!!!
!!!! copyright Victor Eijkhout 2012-2022
!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program VecArray
#include <petsc/finclude/petsc.h>
  use petsc
  implicit none
  
  MPI_Comm       :: comm
  Vec            :: x,y
  PetscInt       :: n=1, procno
  PetscScalar    :: one=1.0, two=2.0, value, inprod,scaling,xnorm,ynorm
  PetscScalar,dimension(:),Pointer :: &
       in_array,out_array
  PetscInt       :: globalsize,localsize,myfirst,mylast,index
  Character*80 :: message
  PetscBool      :: flag
  PetscErrorCode :: ierr
  
  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRA(ierr)
  comm = PETSC_COMM_WORLD
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Get a commandline argument for the size of the problem
  !!
  call PetscOptionsGetInt( &
       PETSC_NULL_OPTIONS,PETSC_NULL_CHARACTER, &
       "-n",n,flag,ierr)
  CHKERRA(ierr)

  !!
  !! Create vector `x' with a default layout
  !!
  call VecCreate(comm,x,ierr); CHKERRA(ierr)
  call VecSetSizes(x,n,PETSC_DECIDE,ierr); CHKERRA(ierr)
  call VecSetFromOptions(x,ierr); CHKERRA(ierr)

  !!
  !! Set x,y to constant values
  !!
  call VecSet(x,one,ierr); CHKERRA(ierr)

  !!
  !! Make another vector, just like x
  !!
  call VecDuplicate(x,y,ierr); CHKERRA(ierr)

  !!
  !! Get arrays and operate on them
  !!
  call VecGetArrayReadF90( x,in_array,ierr )
  call VecGetArrayF90( y,out_array,ierr )
  call VecGetLocalSize( x,localsize,ierr )
  do index=1,localsize
     out_array(index) = 2*in_array(index)
  end do
  call VecRestoreArrayReadF90( x,in_array,ierr )
  call VecRestoreArrayF90( y,out_array,ierr )

  !!
  !! Sanity check printout
  !!
  call VecNorm(x,NORM_2,xnorm,ierr)
  call VecNorm(y,NORM_2,ynorm,ierr)
  write(message,10) xnorm,ynorm
10 format("Norm x: ",f6.3,", y: ",f6.3,"\n")
  call PetscPrintf(comm,message,ierr)

  !!
  !! Free work space. All PETSc objects should be destroyed when they
  !! are no longer needed
  !!
  call VecDestroy(x,ierr)
  call VecDestroy(y,ierr)

  call PetscFinalize(ierr);
  CHKERRA(ierr)
  
END program VecArray
