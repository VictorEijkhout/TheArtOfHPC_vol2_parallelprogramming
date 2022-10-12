/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 ****************************************************************/

#include "petscksp.h"
#include "fivepoint.c"

PetscErrorCode mymatmult(Mat,Vec,Vec);

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  int domain_size,matrix_size;
  PetscErrorCode ierr;
  PetscBool flag;
  MPI_Comm comm;
  KSP Solve;
  Mat AtA,Afivept;
  Vec Rhs,Sol;
  int localsize;
  PetscScalar one = 1.0;
  PetscScalar u=.1;
  
  PetscFunctionBegin;
  PetscInitialize(&argc,&args,0,0);

  comm = MPI_COMM_WORLD;

  /*
    read the domain size, and square it to get the matrix size
  */
  ierr = PetscOptionsGetInt(PETSC_NULL,PETSC_NULL,"-n",&domain_size,&flag); CHKERRQ(ierr);
  if (!flag) domain_size = 10;
  matrix_size = domain_size*domain_size;
  localsize=PETSC_DECIDE;
  ierr = PetscSplitOwnership(comm,&localsize,&matrix_size); CHKERRQ(ierr);

  /*
   * Determine how unsymmetric is the matrix going to be
   */
  ierr = PetscOptionsGetReal(PETSC_NULL,PETSC_NULL,"-u",&u,&flag); CHKERRQ(ierr);
  if (!flag) u = .1;

  /*
    create the five-point laplacian matrix
  */
  ierr = MatCreate(comm,&Afivept); CHKERRQ(ierr);
  ierr = MatSetType(Afivept,MATMPIAIJ); CHKERRQ(ierr);
  ierr = MatSetSizes
    (Afivept,localsize,localsize,matrix_size,matrix_size); CHKERRQ(ierr);
  ierr = MatMPIAIJSetPreallocation(Afivept,5,NULL,2,NULL); CHKERRQ(ierr);
  ierr = FivePointMatrix(comm,domain_size,domain_size,Afivept); CHKERRQ(ierr);
  /* make the matrix nicely unsymmetric */
  ierr = MatSetValue(Afivept,0,matrix_size-1,u,INSERT_VALUES); CHKERRQ(ierr);
  ierr = MatAssemblyBegin(Afivept,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatAssemblyEnd(Afivept,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);

  /*
   * Create a shell matrix, and set its MatMult routine to your own 
   */
  ierr = MatCreate(comm,&AtA); CHKERRQ(ierr);
  ierr = MatSetSizes(AtA,localsize,localsize,matrix_size,matrix_size); CHKERRQ(ierr);
  ierr = MatSetType(AtA,MATSHELL); CHKERRQ(ierr);
  ierr = MatSetFromOptions(AtA); CHKERRQ(ierr);
  ierr = MatShellSetOperation(AtA,MATOP_MULT,(void*)&mymatmult); CHKERRQ(ierr);
  ierr = MatShellSetContext(AtA,(void*)Afivept); CHKERRQ(ierr);
  ierr = MatSetUp(AtA); CHKERRQ(ierr);

  /*
   * create right hand side and solution vectors
   */
  ierr = VecCreateMPI(comm,PETSC_DECIDE,matrix_size,&Rhs); CHKERRQ(ierr);
  ierr = VecDuplicate(Rhs,&Sol); CHKERRQ(ierr);
  ierr = VecSet(Rhs,one); CHKERRQ(ierr);

  /*
   * create iterative method and preconditioner
   */
  ierr = KSPCreate(comm,&Solve);
  ierr = KSPSetOperators(Solve,Afivept,AtA); CHKERRQ(ierr);
  ierr = KSPSetType(Solve,KSPCG); CHKERRQ(ierr);
  {
    PC Prec;
    ierr = KSPGetPC(Solve,&Prec); CHKERRQ(ierr);
    ierr = PCSetType(Prec,PCNONE); CHKERRQ(ierr);
  }
  ierr = KSPSetFromOptions(Solve); CHKERRQ(ierr);

  /*
   * solve the system and analyse the outcome
   */
  ierr = KSPSolve(Solve,Rhs,Sol); CHKERRQ(ierr);
  {
    PetscInt its; KSPConvergedReason reason; 
    Vec Res; PetscReal norm;
    ierr = KSPGetConvergedReason(Solve,&reason);
    if (reason<0) {
      PetscPrintf(comm,"Failure to converge\n");
    } else {
      ierr = KSPGetIterationNumber(Solve,&its); CHKERRQ(ierr);
      PetscPrintf(comm,"Number of iterations: %d\n",its);
    }
    ierr = VecDuplicate(Rhs,&Res); CHKERRQ(ierr);
    ierr = MatMult(AtA,Sol,Res); CHKERRQ(ierr);
    ierr = VecAXPY(Res,-1,Rhs); CHKERRQ(ierr);
    ierr = VecNorm(Res,NORM_2,&norm); CHKERRQ(ierr);
    ierr = PetscPrintf(MPI_COMM_WORLD,"residual norm: %e\n",norm); CHKERRQ(ierr);
    ierr = VecDestroy(&Res); CHKERRQ(ierr);
  }

  /*
   * clean up
   */
  ierr = KSPDestroy(&Solve); CHKERRQ(ierr);
  ierr = VecDestroy(&Rhs); CHKERRQ(ierr);
  ierr = VecDestroy(&Sol); CHKERRQ(ierr);
  
  ierr = PetscFinalize();
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "mymatmult"
PetscErrorCode mymatmult(Mat mat,Vec in,Vec out)
{
  Mat A; PetscErrorCode ierr;
  Vec t;
  PetscFunctionBegin;
  ierr = VecDuplicate(in,&t); CHKERRQ(ierr);
  ierr = MatShellGetContext(mat,(void**)&A); CHKERRQ(ierr);
  ierr = MatMult(A,in,t);
  CHKMEMQ;
  ierr = MatMultTranspose(A,t,out); CHKERRQ(ierr);
  ierr = VecDestroy(&t); CHKERRQ(ierr);
  PetscFunctionReturn(0);
}
