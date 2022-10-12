! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the tutorial
!**** `Introduction to the PETSc library'
!**** by Victor Eijkhout eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2020
!****
!**** backtrace.F90 : demonstrate backtracing error messages
!****
!****************************************************************

#define PETSC_USE_FORTRAN_MODULES 1
#include "petsc/finclude/petsc.h"

#undef __FUNCT__
#define __FUNCT__ "main"
  program shell
    use petsc
    use fivepoint
    implicit none

      EXTERNAL mymatmult
      common /savedmat/common_matrix
      Mat common_matrix

      integer :: dom_size,matrix_size,localsize,N
      PetscErrorCode ierr
      PetscBool flag
      MPI_Comm comm
      KSP Solve
      PC Prec
      Mat Afivept,AtA
      Vec Rhs,Sol
      PetscInt :: its
      KSPConvergedReason reason 
      Vec Res
      PetscReal norm
      PetscScalar one,mone
      PetscScalar u
  
      call PetscInitialize(PETSC_NULL_CHARACTER,ierr) 
      CHKERRQ(ierr)
      one = 1.0 ; mone = -1.0
      comm = MPI_COMM_WORLD

!!
!! Read the domain size, and square it to get the matrix size
!!
      call PetscOptionsGetInt(PETSC_NULL_OPTIONS,PETSC_NULL_CHARACTER, &
          "-n",dom_size,flag,ierr) 
      if (.not.flag) dom_size = 10
      matrix_size = dom_size*dom_size
      localsize = PETSC_DECIDE
      call PetscSplitOwnership(comm,localsize,matrix_size,ierr)

      !!
      !! Create the five-point laplacian matrix
      !!
      call MatCreate(comm,Afivept,ierr)
      call MatSetType(Afivept,MATMPIAIJ,ierr)
      call MatSetSizes(Afivept,PETSC_DECIDE,PETSC_DECIDE, &
          matrix_size,matrix_size,ierr)
      call MatMPIAIJSetPreallocation(Afivept,&
           5,PETSC_NULL_INTEGER,2,PETSC_NULL_INTEGER,ierr)
      call FivePointMatrix(comm,dom_size,dom_size,Afivept,ierr)
      !! make nice and unsymmetric
      call MatSetValue(Afivept,0,matrix_size-1,u,INSERT_VALUES,ierr)
      call MatAssemblyBegin(Afivept,MAT_FINAL_ASSEMBLY,ierr)
      call MatAssemblyEnd(Afivept,MAT_FINAL_ASSEMBLY,ierr)

      !!
      !! Create a shell matrix and set its MatMult routine
      !!
      call MatCreate(comm,AtA,ierr)
      call MatSetSizes(AtA,localsize,localsize,matrix_size,matrix_size,ierr)
      call MatSetType(AtA,MATSHELL,ierr)
      call MatSetFromOptions(AtA,ierr)
      call MatShellSetOperation(AtA,MATOP_MULT,mymatmult,ierr)
      !! no context in Fortran, instead
      common_matrix = Afivept
      call MatSetUp(AtA,ierr)

      !call MatView(A,PETSC_VIEWER_STDOUT_WORLD,ierr)

!  /*
!   * create right hand side and solution vectors
!   */
      call VecCreateMPI(comm,PETSC_DECIDE,matrix_size,Rhs,ierr)
      call VecDuplicate(Rhs,Sol,ierr)
      call VecSet(Rhs,one,ierr)

!  /*
!   * create iterative method and preconditioner
!   */
      call KSPCreate(comm,Solve,ierr)
      CHKERRQ(ierr)
      call KSPSetOperators(Solve,AtA,Afivept,ierr)
      CHKERRQ(ierr)
      call KSPSetType(Solve,KSPCG,ierr)
      CHKERRQ(ierr)

      call KSPGetPC(Solve,Prec,ierr)
      CHKERRQ(ierr)
      call PCSetType(Prec,PCNONE,ierr)
      CHKERRQ(ierr)
  
      call KSPSetFromOptions(Solve,ierr)
      CHKERRQ(ierr)

!  /*
!   * solve the system and analyse the outcome
!   */
      call KSPSolve(Solve,Rhs,Sol,ierr)
      CHKERRQ(ierr)

      call KSPGetConvergedReason(Solve,reason,ierr)
      if (reason<0) then
         call PetscPrintf(comm,"Failure to converge")
         CHKERRQ(ierr)
      else 
         call KSPGetIterationNumber(Solve,its,ierr)
      CHKERRQ(ierr)
         print *,"Number of iterations:",its
      end if

      call VecDuplicate(Rhs,Res,ierr)
      CHKERRQ(ierr)
      call MatMult(AtA,Sol,Res,ierr)
      CHKERRQ(ierr)
      call VecAXPY(Res,mone,Rhs,ierr)
      CHKERRQ(ierr)
      call VecNorm(Res,NORM_2,norm,ierr)
      CHKERRQ(ierr)
      print *,'residual norm',norm
      call VecDestroy(Res,ierr)
      CHKERRQ(ierr)
  

!  /*
!   * clean up
!   */
      call KSPDestroy(Solve,ierr)
      CHKERRQ(ierr)
      call VecDestroy(Rhs,ierr)
      CHKERRQ(ierr)
      call VecDestroy(Sol,ierr)
      CHKERRQ(ierr)
  
      call PetscFinalize(ierr)
      end program shell

#undef __FUNCT__
#define __FUNCT__ "mymatmult"
      subroutine mymatmult(mat,in,out,ierr)
        use petsc
        implicit none
        Mat mat,common_matrix
        Vec in, out
        PetscErrorCode ierr
        common /savedmat/common_matrix

! only in C; in Fortran we use a common block
!      call MatShellGetContext(mat,(void**)A,ierr) 
        call MatMult(common_matrix,in,out,ierr)
        call MatMultTransposeAdd(common_matrix,in,out,out,ierr)

      return
      end subroutine
