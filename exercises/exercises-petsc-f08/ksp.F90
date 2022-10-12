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

module fivepoint

  !!
  !! Creates the five pointeger central difference matrix
  !! on a domain of mxn points, that is: the matrix
  !! is of size mn 
  !!
contains

#undef __FUNCT__
#define __FUNCT__ "FivePointMatrix"
  subroutine FivePointMatrix(comm,domain_m,domain_n,unsymmetry,A,ierr)
#include <petsc/finclude/petsc.h>
    use petsc
    implicit none

    !! Parameters
    MPI_Comm,intent(in)         :: comm
    PetscInt,intent(in)         :: domain_m,domain_n
    double precision,intent(in) :: unsymmetry
    Mat,intent(inout)           :: A
    PetscErrorCode,intent(out)  :: ierr

    !! Local variables
    integer             :: matrix_size, first,last
    integer             :: i,j, iglobal,jglobal
    PetscScalar         :: v

    !!
    !! Create the five-point laplacian matrix
    !!
    call MatCreate(comm,A,ierr)
    CHKERRQ(ierr)
    call MatSetType(A,MATMPIAIJ,ierr)
    CHKERRQ(ierr)
    matrix_size = domain_m * domain_n
    call MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,&
         matrix_size,matrix_size,ierr)
    CHKERRQ(ierr)
    call MatMPIAIJSetPreallocation(A, 5,PETSC_NULL_INTEGER, 2,PETSC_NULL_INTEGER,ierr)
    CHKERRQ(ierr)
    call MatSetOption(A, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE,ierr)
    CHKERRQ(ierr)
    call MatSetFromOptions(A,ierr)
    CHKERRQ(ierr)

    call MatAssemblyBegin(A,MAT_FLUSH_ASSEMBLY,ierr)
    CHKERRQ(ierr)
    call MatAssemblyEnd(A,MAT_FLUSH_ASSEMBLY,ierr)

    call MatGetOwnershipRange(A,first,last,ierr)

    do i=0,domain_m-1
       do j=0,domain_n-1
          iglobal = j + domain_n*i
          if (iglobal<first .or. iglobal>=last) &
               cycle

          !! diagonal element
          v = 4.0d0
          call MatSetValue(A,iglobal,iglobal,v,INSERT_VALUES,ierr)
          CHKERRQ(ierr)

          !! previous row; we set an amount of unsymmetry if this is a periodic point
          v = -1.0d0; jglobal = iglobal - domain_n
          if ( i>0 ) then
             call MatSetValue(A,iglobal,jglobal,v,INSERT_VALUES,ierr); CHKERRQ(ierr)
          else
             jglobal = jglobal+matrix_size; v = -unsymmetry
             call MatSetValue(A,iglobal,jglobal,v,INSERT_VALUES,ierr)
             CHKERRQ(ierr)
          end if

          !! next row; we set an amount of unsymmetry if this is a periodic point
          v = -1.0d0; jglobal = iglobal + domain_n
          if ( i<domain_m-1 ) then
             call MatSetValue(A,iglobal,jglobal,v,INSERT_VALUES,ierr); CHKERRQ(ierr)
          else
             jglobal = jglobal-matrix_size; v = +unsymmetry
             call MatSetValue(A,iglobal,jglobal,v,INSERT_VALUES,ierr)
             CHKERRQ(ierr)
          end if
          
          !! previous point; we set an amount of unsymmetry if this is a periodic point
          v = -1.0d0; jglobal = iglobal - 1
          if ( j>0 ) then
             call MatSetValue(A,iglobal,jglobal,v,INSERT_VALUES,ierr); CHKERRQ(ierr)
          else
             jglobal = jglobal+domain_n; v = -unsymmetry
             call MatSetValue(A,iglobal,jglobal,v,INSERT_VALUES,ierr)
             CHKERRQ(ierr)
          end if

          !! next point; we set an amount of unsymmetry if this is a periodic point
          v = -1.0d0; jglobal = iglobal + 1
          if ( j<domain_n-1 ) then
             call MatSetValue(A,iglobal,jglobal,v,INSERT_VALUES,ierr); CHKERRQ(ierr)
          else
             jglobal = jglobal-domain_n; v = +unsymmetry
             call MatSetValue(A,iglobal,jglobal,v,INSERT_VALUES,ierr)
             CHKERRQ(ierr)
          end if
       end do
    end do
    call MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY,ierr); CHKERRQ(ierr)
    call MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY,ierr); CHKERRQ(ierr)

    return
  end subroutine FivePointMatrix

end module fivepoint

program ksp
#include <petsc/finclude/petsc.h>
#include <petscversion.h>
  use petsc
  use fivepoint
  implicit none

  PetscInt :: dom_size,matrix_size,N,procno
  character*100 textbuf
  PetscErrorCode ierr
  PetscBool flag
  MPI_Comm comm
  KSP Solver
  PC Prec
  PetscInt           :: localsize,its
  KSPConvergedReason reason 
  Vec Res
  PetscReal          :: unsymmetry = 0.d0
  Mat A
  Vec Rhs,Sol
  PetscScalar one,mone

  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  one = 1.0; mone = -1.0
  comm = MPI_COMM_WORLD
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Read the domain size, and square it to get the matrix size
  !!
  call PetscOptionsGetInt( &
       PETSC_NULL_OPTIONS, &
       PETSC_NULL_CHARACTER,"-n",dom_size,flag,ierr)
  if (.not.flag) dom_size = 100
  if (procno==0) print *,"Using domain size",dom_size

  !!
  !! Read the amount of unsymmetry to be added to the matrix
  !!
  unsymmetry = 0.d0
  call PetscOptionsGetReal( &
     PETSC_NULL_OPTIONS, &
     PETSC_NULL_CHARACTER,"-unsymmetry",unsymmetry,flag,ierr)

  !!
  !! Create the five-point laplacian matrix, with unsymmetry
  !!
  call FivePointMatrix(comm,dom_size,dom_size,unsymmetry,A,ierr)
  CHKERRQ(ierr)
!!  call MatView(A,PETSC_VIEWER_STDOUT_WORLD,ierr)

  !!
  !! Create right hand side and solution vectors
  !!
  call MatGetLocalSize(A,localsize,PETSC_NULL_INTEGER,ierr)
  call VecCreateMPI(comm,localsize,PETSC_DECIDE,Rhs,ierr)
  call VecDuplicate(Rhs,Sol,ierr); CHKERRQ(ierr)
  call VecSet(Rhs,one,ierr); CHKERRQ(ierr)

  !!
  !! Create iterative method and preconditioner
  !!
  call KSPCreate(comm,Solver,ierr)
  call KSPSetOperators(Solver,A,A,ierr); CHKERRQ(ierr)
  call KSPSetType(Solver,KSPCG,ierr); CHKERRQ(ierr)

  call KSPGetPC(Solver,Prec,ierr); CHKERRQ(ierr)
  call PCSetType(Prec,PCJACOBI,ierr); CHKERRQ(ierr)

  !!
  !! Incorporate any commandline options for the KSP
  !!
  call KSPSetFromOptions(Solver,ierr); CHKERRQ(ierr)

  !!
  !! Solve the system and analyse the outcome
  !!
  call KSPSolve(Solver,Rhs,Sol,ierr); CHKERRQ(ierr)

  call KSPGetConvergedReason(Solver,reason,ierr)
  if (reason<0) then
     write(textbuf,20) reason
20   format("Failure to converge",i3,".\n")
     call PetscPrintf(comm,textbuf,ierr); CHKERRQ(ierr)
  else
     call KSPGetIterationNumber(Solver,its,ierr)
     CHKERRQ(ierr)
     write(textbuf,10) its
10   format("Converged in",I5," iterations.\n")
     call PetscPrintf(comm,textbuf,ierr); CHKERRQ(ierr)
  end if

  !!
  !! Exercise: Investigate the convergence behaviour with different KSP/PC values.
  !! you can find the possible values in $PETSC_DIR/include/: petscksp.h and petscpctypes.h
  !!
  !!    To get feedback on your choices use the following options:
  !!    -ksp_monitor : print the progress of the iterative method
  !!    -ksp_view    : show the details of the solver
  !!    -mat_view    : ascii display of the coefficient matrix.
  !!
  !! Now do the following experiments:
  !! -- try a range of problem sizes with "-ksp_type cg" vs "bcgs" or "gmres"
  !! -- set the unsymmetry parameter to something nonzero: "-unsymmetry 1"; higher/lower?
  !!    (if you know what convection/diffusion is, how should n/unsymmetry relate?)
  !! -- both for the symmetric and unsymmetric case, how does convergence vary with
  !!    the number of processes by given problem size?
  !! -- with unsymmetry, the iterative methods may fail.
  !!    Add a non-trivial precondition: "-pc_type bjacobi" or "asm"
  !!    these preconditioners have a sub-pc: "-sub_pc_type ilu" vs "lu"
  !!    (If you want to see the actual convergence, add "-ksp_monitor")
  !!
  !! Analyze the cost of various operation by running with "-log_summary". 
  !! -- what is the time in PCSetUp for "jacobi" vs "bjacobi" with "ilu" or "asm" with "lu"?
  !! -- how many matrix-vector products per iteration in cg? bicg? bcgs?
  !! -- what mega/gigafloppage are you getting? are you happy with the number?
  !!

  !!
  !! Clean up allocated objects
  !!
  call MatDestroy(A,ierr); CHKERRQ(ierr)
  call KSPDestroy(Solver,ierr); CHKERRQ(ierr)
  call VecDestroy(Rhs,ierr); CHKERRQ(ierr)
  call VecDestroy(Sol,ierr); CHKERRQ(ierr)
  !      call VecDestroy(Res,ierr) 
  !      CHKERRQ(ierr)

  call PetscFinalize(ierr)

end program ksp
