/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2024
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
  PetscCall( PetscOptionsGetInt(PETSC_NULL,PETSC_NULL,"-n",&domain_size,&flag) ); 
  if (!flag) domain_size = 10;
  matrix_size = domain_size*domain_size;
  localsize=PETSC_DECIDE;
  PetscCall( PetscSplitOwnership(comm,&localsize,&matrix_size) ); 

  /*
   * Determine how unsymmetric is the matrix going to be
   */
  PetscCall( PetscOptionsGetReal(PETSC_NULL,PETSC_NULL,"-u",&u,&flag) ); 
  if (!flag) u = .1;

  /*
    create the five-point laplacian matrix
  */
  PetscCall( MatCreate(comm,&Afivept) ); 
  PetscCall( MatSetType(Afivept,MATMPIAIJ) ); 
  PetscCall( MatSetSizes(Afivept,localsize,localsize,matrix_size,matrix_size) ); 
  PetscCall( MatMPIAIJSetPreallocation(Afivept,5,NULL,2,NULL) ); 
  PetscCall( FivePointMatrix(comm,domain_size,domain_size,Afivept) ); 
  /* make the matrix nicely unsymmetric */
  PetscCall( MatSetValue(Afivept,0,matrix_size-1,u,INSERT_VALUES) ); 
  PetscCall( MatAssemblyBegin(Afivept,MAT_FINAL_ASSEMBLY) ); 
  PetscCall( MatAssemblyEnd(Afivept,MAT_FINAL_ASSEMBLY) ); 

  /*
   * Create a shell matrix, and set its MatMult routine to your own 
   */
  PetscCall( MatCreate(comm,&AtA) ); 
  PetscCall( MatSetSizes(AtA,localsize,localsize,matrix_size,matrix_size) ); 
  PetscCall( MatSetType(AtA,MATSHELL) ); 
  PetscCall( MatSetFromOptions(AtA) ); 
  PetscCall( MatShellSetOperation(AtA,MATOP_MULT,(void*)&mymatmult) ); 
  PetscCall( MatShellSetContext(AtA,(void*)Afivept) ); 
  PetscCall( MatSetUp(AtA) ); 

  /*
   * create right hand side and solution vectors
   */
  PetscCall( VecCreateMPI(comm,PETSC_DECIDE,matrix_size,&Rhs) ); 
  PetscCall( VecDuplicate(Rhs,&Sol) ); 
  PetscCall( VecSet(Rhs,one) ); 

  /*
   * create iterative method and preconditioner
   */
  PetscCall( KSPCreate(comm,&Solve) );
  PetscCall( KSPSetOperators(Solve,Afivept,AtA) ); 
  PetscCall( KSPSetType(Solve,KSPCG) ); 
  {
    PC Prec;
    PetscCall( KSPGetPC(Solve,&Prec) ); 
    PetscCall( PCSetType(Prec,PCNONE) ); 
  }
  PetscCall( KSPSetFromOptions(Solve) ); 

  /*
   * solve the system and analyse the outcome
   */
  PetscCall( KSPSolve(Solve,Rhs,Sol) ); 
  {
    PetscInt its; KSPConvergedReason reason; 
    Vec Res; PetscReal norm;
    PetscCall( KSPGetConvergedReason(Solve,&reason) );
    if (reason<0) {
      PetscPrintf(comm,"Failure to converge\n");
    } else {
      PetscCall( KSPGetIterationNumber(Solve,&its) ); 
      PetscPrintf(comm,"Number of iterations: %d\n",its);
    }
    PetscCall( VecDuplicate(Rhs,&Res) ); 
    PetscCall( MatMult(AtA,Sol,Res) ); 
    PetscCall( VecAXPY(Res,-1,Rhs) ); 
    PetscCall( VecNorm(Res,NORM_2,&norm) ); 
    PetscCall( PetscPrintf(MPI_COMM_WORLD,"residual norm: %e\n",norm) ); 
    PetscCall( VecDestroy(&Res) ); 
  }

  /*
   * clean up
   */
  PetscCall( KSPDestroy(&Solve) ); 
  PetscCall( VecDestroy(&Rhs) ); 
  PetscCall( VecDestroy(&Sol) ); 
  
  PetscFinalize();
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "mymatmult"
PetscErrorCode mymatmult(Mat mat,Vec in,Vec out)
{
  Mat A; PetscErrorCode ierr;
  Vec t;
  PetscFunctionBegin;
  PetscCall( VecDuplicate(in,&t) ); 
  PetscCall( MatShellGetContext(mat,(void**)&A) ); 
  PetscCall( MatMult(A,in,t) );
  CHKMEMQ;
  PetscCall( MatMultTranspose(A,t,out) ); 
  PetscCall( VecDestroy(&t) ); 
  PetscFunctionReturn(0);
}
