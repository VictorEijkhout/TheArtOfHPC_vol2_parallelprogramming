/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 ****************************************************************/

#include "petscksp.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  PetscErrorCode ierr;
  MPI_Comm comm;
  KSP Solver;
  Mat A;
  Vec Rhs,Sol;
  PetscScalar one = 1.0;
  
  PetscFunctionBegin;
  PetscInitialize(&argc,&args,0,0);

  comm = PETSC_COMM_WORLD;
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  /*
   * Read the domain size, and square it to get the matrix size
  */
  PetscBool flag;
  int matrix_size = 100; 
  ierr = PetscOptionsGetInt
    (NULL,PETSC_NULL,"-n",&matrix_size,&flag); CHKERRQ(ierr);
  PetscPrintf(comm,"Using local matrix size %d\n",matrix_size);

  /*
   * Create the five-point laplacian matrix
  */
  ierr = MatCreate(comm,&A); CHKERRQ(ierr);
  ierr = MatSetType(A,MATMPIAIJ); CHKERRQ(ierr);
  ierr = MatSetSizes(A,matrix_size,matrix_size,PETSC_DECIDE,PETSC_DECIDE); CHKERRQ(ierr);
  ierr = MatMPIAIJSetPreallocation(A,5,PETSC_NULL,3,PETSC_NULL); CHKERRQ(ierr);
  ierr = MatCreateVecs(A,&Rhs,PETSC_NULL); CHKERRQ(ierr);
  int first,last;
  ierr = MatGetOwnershipRange(A,&first,&last); CHKERRQ(ierr);
  for (int i=first; i<last; i++) {
    PetscScalar
      h = 1./(matrix_size+1), pi = 3.1415926,
      sin1 = i * pi * h, sin2 = 2 * i * pi * h, sin3 = 3 * i * pi * h,
      coefs[3] = {-1,2,-1};
    PetscInt cols[3] = {i-1,i,i+1};
    ierr = VecSetValue(Rhs,i,sin1 + .5 * sin2 + .3 * sin3, INSERT_VALUES); CHKERRQ(ierr);
    if (i==0) {
      ierr = MatSetValues(A,1,&i,2,cols+1,coefs+1,INSERT_VALUES); CHKERRQ(ierr);
    } else if (i==matrix_size-1) {
      ierr = MatSetValues(A,1,&i,2,cols,coefs,INSERT_VALUES); CHKERRQ(ierr);
    } else {
      ierr = MatSetValues(A,1,&i,2,cols,coefs,INSERT_VALUES); CHKERRQ(ierr);
    }
  }
  ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  //MatView(A,PETSC_VIEWER_STDOUT_WORLD);
  ierr = VecAssemblyBegin(Rhs); CHKERRQ(ierr);
  ierr = VecAssemblyEnd(Rhs); CHKERRQ(ierr);

  /*
   * Create right hand side and solution vectors
   */
  ierr = VecDuplicate(Rhs,&Sol); CHKERRQ(ierr);
  ierr = VecSet(Rhs,one); CHKERRQ(ierr);

  /*
   * Create iterative method and preconditioner
   */
  ierr = KSPCreate(comm,&Solver);
  ierr = KSPSetOperators(Solver,A,A); CHKERRQ(ierr);
  ierr = KSPSetType(Solver,KSPPREONLY); CHKERRQ(ierr);
  {
    PC Prec;
    ierr = KSPGetPC(Solver,&Prec); CHKERRQ(ierr);
    ierr = PCSetType(Prec,PCLU); CHKERRQ(ierr);
    ierr = PCFactorSetMatSolverType(Prec,MATSOLVERMUMPS); CHKERRQ(ierr);
  }

  /*
   * Incorporate any commandline options for the KSP
   */
  ierr = KSPSetFromOptions(Solver); CHKERRQ(ierr);

  /*
   * Solve the system and analyze the outcome
   */
  ierr = KSPSolve(Solver,Rhs,Sol); CHKERRQ(ierr);
  {
    PetscInt its; KSPConvergedReason reason; 
    ierr = KSPGetConvergedReason(Solver,&reason);
    ierr = KSPGetIterationNumber(Solver,&its); CHKERRQ(ierr);
    if (reason<0) {
      PetscPrintf(comm,"Failure to converge after %d iterations; reason %s\n",
		  its,KSPConvergedReasons[reason]);
    } else {
      PetscPrintf(comm,"Number of iterations to convergence: %d\n",its);
    }
  }

  ierr = MatDestroy(&A); CHKERRQ(ierr);
  ierr = KSPDestroy(&Solver); CHKERRQ(ierr);
  ierr = VecDestroy(&Rhs); CHKERRQ(ierr);
  ierr = VecDestroy(&Sol); CHKERRQ(ierr);
  
  return PetscFinalize();
}

