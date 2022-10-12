! -*- f90 -*-
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!
!!!! This program file is part of the tutorial
!!!! `Introduction to the PETSc library'
!!!! by Victor Eijkhout eijkhout@tacc.utexas.edu
!!!!
!!!! copyright Victor Eijkhout 2012-8
!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!!
!! Function to create the matrix.
!! Let's pretend that you do not know what happens here,
!! so you have no information about the matrix size and such.
!!
subroutine CreateMatrix(comm,nlocal,rA)
#include <petsc/finclude/petsc.h>
  use petsc
  implicit none

  !! parameters
  MPI_Comm,intent(in) :: comm
  integer,intent(in)  :: nlocal
  Mat,intent(out)     :: rA

  !! local variables
  Mat            :: A
  PetscInt       :: first,last, row,col
  PetscScalar    :: v
  PetscErrorCode :: ierr
  integer        :: rank,nglobal, usenlocal

  usenlocal = nlocal
  call MatCreate(comm,A,ierr)
  call MatSetType(A,MATMPIAIJ,ierr)

  call MPI_Comm_rank(comm,rank,ierr)
  if (rank==0) &
      usenlocal = 1
  call MatSetSizes(A,usenlocal,usenlocal,PETSC_DECIDE,PETSC_DECIDE,ierr)
  call MatMPIAIJSetPreallocation(A, &
       3,PETSC_NULL_INTEGER,1,PETSC_NULL_INTEGER, &
       ierr)

  call MatAssemblyBegin(A,MAT_FLUSH_ASSEMBLY,ierr)
  call MatAssemblyEnd(A,MAT_FLUSH_ASSEMBLY,ierr)

  call MatGetSize(A,nglobal,PETSC_NULL_INTEGER,ierr)
  call MatGetOwnershipRange(A,first,last,ierr)
  do row=first,last-1
     col = row; v = 2.
     call MatSetValue(A,row,col,v,INSERT_VALUES,ierr)
     col = row-1; v = -1.
     if (col>=0) &
          call MatSetValue(A,row,col,v,INSERT_VALUES,ierr)
     col = row+1; v = -1.
     if (col<nglobal) &
          call MatSetValue(A,row,col,v,INSERT_VALUES,ierr)
  end do
  call MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY,ierr)
  call MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY,ierr)

  rA = A

  return
end subroutine CreateMatrix

program MatVec
#include <petsc/finclude/petsc.h>
#include <petscversion.h>
  use petsc
  implicit none
  
  MPI_Comm       :: comm
  Mat            :: A
  Vec            :: x,y, localvec
  integer        :: procno,nprocs
  PetscInt       :: nlocal = 20, rowsize, localsize
  PetscScalar    :: one = 1.
  PetscScalar,pointer :: localdata(:)
  PetscReal      :: norm, norm_shouldbe
  PetscBool      :: flag
  PetscErrorCode :: ierr
  
  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRQ(ierr)
  comm = PETSC_COMM_WORLD
  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Get a commandline argument for the size of the problem
  !!
  call PetscOptionsGetInt( &
       PETSC_NULL_OPTIONS, PETSC_NULL_CHARACTER, &
       "-n",nlocal,flag,ierr)

  !!
  !! Call the matrix creation routine
  !!
  call CreateMatrix(comm,nlocal,A)
  !! just to check
  !!call MatView(A,PETSC_VIEWER_STDOUT_WORLD,ierr)

  !!
  !! Start creating a vector.
  !!
  call VecCreate(comm,x,ierr)
  call VecSetType(x,VECMPI,ierr)

  !!
  !! Exercise:
  !! -- find the local size of the matrix, and use that to set the vector size
  !!
  call MatGetLocalSize(A, &
!!!! your code here !!!!
       ierr)
  call VecSetSizes(x, &
!!!! your code here !!!!
       ierr)

  !!
  !! Duplicate a vector to get the same format and partitioning
  !!
  call VecDuplicate(x,y,ierr)

  !!
  !! Set x to constant one
  !!
  call VecSet(x,one,ierr)

  !!
  !! Exercise:
  !! -- fill in the correct parameters to compute
  !!    the matrix vector product y = Ax
  !!
  call MatMult( &
!!!! your code here !!!!
       ierr)

  !!
  !! First check on the product
  !!
  call VecNorm(y,NORM_1,norm,ierr)
  if ( abs(2.-norm)>1.e-14 ) then
     if (procno==0) &
          print *,"Wrong norm",norm,"should be 2."
  else
     if (procno==0) &
          print *,"Global norm test succeeded"
  end if
  
!!#if 0
  !!
  !! Second, more funky test
  !!
  call VecCreate(PETSC_COMM_SELF,localvec,ierr)
  call VecSetType(localvec,VECSEQ,ierr)
  call VecGetLocalSize(y,localsize,ierr)
  call VecSetSizes(localvec,localsize,PETSC_DECIDE,ierr)
  !!
  !! Exercise:
  !! -- put y's data into the local vector, and compute the local norm
  !! -- hint: use VecGetArrayF90 to get the y data
  !! --       use VecPlaceArray to set that in localvec
  !!
!!!! your code here !!!!

  !!
  !! Compute the norm: it should be 1 on the first & last process
  !! zero elsewhere
  !!
  call VecNorm(localvec,NORM_1,norm,ierr)
  if (procno==0 .or. procno==nprocs-1) then
     norm_shouldbe = 1.
  else
     norm_shouldbe = 0.
  end if
  if ( abs(norm-norm_shouldbe)>1.e-14 ) then
     print *,"Wrong local norm",norm,"on rank",procno
  else
     print *,"Local norm succeeded on all",nprocs
  end if
  
  !!
  !! Optional exercise:
  !! -- what are the mirror routines of VecGetArray and VecPlaceArray above?
  !!    (this mostly serves to prevent memory leaks)
  !!
!!!! your code here !!!!
  call VecDestroy(localvec,ierr)

!!#endif

  !!
  !! Free workspace. All PETSc objects should be destroyed when they
  !! are no longer needed.
  !!
  call MatDestroy(A,ierr)
  call VecDestroy(x,ierr)
  call VecDestroy(y,ierr)

  call PetscFinalize(ierr)
  CHKERRQ(ierr)
  
END program MatVec

