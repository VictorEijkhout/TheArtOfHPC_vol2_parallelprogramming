/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2023
 ****
 ****************************************************************/

#include "petscksp.h"

PetscErrorCode FivePointMatrix(MPI_Comm,PetscInt,PetscInt,PetscScalar,Mat*);

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
  PetscInitialize(&argc,&args,0,"ksp -n domainsize -unsymmetry x\n" );

  comm = MPI_COMM_WORLD;

  /*
   * Read the domain size, and square it to get the matrix size
  */
  PetscBool flag;
  PetscInt domain_size = 100; 
  PetscCall( PetscOptionsGetInt(NULL,NULL,"-n",&domain_size,&flag) ); 
  PetscPrintf(comm,"Using domain size %d\n",domain_size);

  /*
   * Read the amount of umsymmetry to be added to the matrix
   */
  PetscReal real_unsymmetry=0.;
  PetscCall( PetscOptionsGetReal(NULL,NULL,"-unsymmetry",&real_unsymmetry,&flag) ); 
  PetscScalar unsymmetry=real_unsymmetry;

  /*
   * Create the five-point laplacian matrix, with unsymmetry.
  */
  PetscCall( FivePointMatrix(comm,domain_size,domain_size,unsymmetry,&A) ); 
  //MatView(A,PETSC_VIEWER_STDOUT_WORLD);

  /*
   * Create right hand side and solution vectors
   */
  PetscInt localsize;
  PetscCall( MatGetLocalSize(A,&localsize,NULL) ); 
  PetscCall( VecCreateMPI(comm,localsize,PETSC_DECIDE,&Rhs) ); 
  PetscCall( VecDuplicate(Rhs,&Sol) ); 
  PetscCall( VecSet(Rhs,one) ); 

  /*
   * Create iterative method and preconditioner
   */
  PetscCall( KSPCreate(comm,&Solver) );
  PetscCall( KSPSetOperators(Solver,A,A) ); 
  PetscCall( KSPSetType(Solver,KSPCG) ); 
  {
    PC Prec;
    PetscCall( KSPGetPC(Solver,&Prec) ); 
    PetscCall( PCSetType(Prec,PCJACOBI) ); 
  }

  /*
   * Incorporate any commandline options for the KSP
   */
  PetscCall( KSPSetFromOptions(Solver) ); 

  /*
   * Solve the system and analyze the outcome
   */
  PetscCall( KSPSolve(Solver,Rhs,Sol) ); 
  {
    PetscInt its; KSPConvergedReason reason; 
    PetscCall( KSPGetConvergedReason(Solver,&reason) );
    PetscCall( KSPGetIterationNumber(Solver,&its) ); 
    if (reason<0) {
      PetscPrintf(comm,"Failure to converge after %d iterations; reason %s\n",
		  its,KSPConvergedReasons[reason]);
    } else {
      PetscPrintf(comm,"Number of iterations to convergence: %d\n",its);
    }
  }

  /*
   * Exercise: Investigate the convergence behaviour with different KSP/PC values.
   * You can find the possible values in $PETSC_DIR/include/: petscksp.h and petscpctypes.h
   *
   *    To get feedback on your choices use the following options:
   *    -ksp_monitor : print the progress of the iterative method
   *    -ksp_view    : show the details of the solver
   *    -mat_view    : ascii display of the coefficient matrix.
   *
   * Now do the following experiments:
   * -- try a range of problem sizes with "-ksp_type cg" vs "bcgs" or "gmres"
   * -- set the unsymmetry parameter to something nonzero: "-unsymmetry 1"; higher/lower?
   *    (if you know what convection/diffusion is, how should n/unsymmetry relate?)
   * -- both for the symmetric and unsymmetric case, how does convergence vary with
   *    the number of processes by given problem size?
   * -- with unsymmetry, the iterative methods may fail.
   *    Add a non-trivial precondition: "-pc_type bjacobi" or "asm"
   *    these preconditioners have a sub-pc: "-sub_pc_type ilu" vs "lu"
   *
   * Analyze the cost of various operation by running with "-log_summary". 
   * -- what is the time in PCSetUp for "jacobi" vs "bjacobi" with "ilu" or "asm" with "lu"?
   * -- how many matrix-vector products per iteration in cg? bicg? bcgs?
   * -- what mega/gigafloppage are you getting? are you happy with the number?
   */

  /*
   * Clean up allocated objects.
   */
  PetscCall( MatDestroy(&A) ); 
  PetscCall( KSPDestroy(&Solver) ); 
  PetscCall( VecDestroy(&Rhs) ); 
  PetscCall( VecDestroy(&Sol) ); 
  
  PetscFinalize();
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "FivePointMatrix"
/* 
 * Create the five point central difference matrix
 * on a domain of mxn points, that is: the matrix is of size mn.
 *
 * We have an `unsymmetry' parameter: higher values are harder on the iterative method. 
 */
PetscErrorCode FivePointMatrix
    (MPI_Comm comm,PetscInt domain_m,PetscInt domain_n,PetscScalar unsymmetry,Mat *rA)
{
  PetscInt i,j;
  PetscErrorCode ierr;
  Mat A;

  PetscFunctionBegin;

  /*
   * Do the basic creation of the matrix
   */
  PetscCall( MatCreate(comm,&A) ); 
  PetscCall( MatSetType(A,MATMPIAIJ) ); 
  PetscInt matrix_size = domain_m*domain_n;
  PetscCall( MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,matrix_size,matrix_size) ); 
  PetscCall( MatMPIAIJSetPreallocation(A,5,NULL,2,NULL) ); 
  PetscCall( MatSetOption(A, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE) ); 
  PetscCall( MatSetFromOptions(A) ); 

  PetscCall( MatAssemblyBegin(A,MAT_FLUSH_ASSEMBLY) ); 
  PetscCall( MatAssemblyEnd(A,MAT_FLUSH_ASSEMBLY) ); 

  /*
   * Fill in the matrix elements
   */
  PetscInt first,last;
  PetscCall( MatGetOwnershipRange(A,&first,&last) ); 
  for ( i=0; i<domain_m; i++ ) {
    for ( j=0; j<domain_n; j++ ) {
      PetscInt iglobal = j + domain_m*i, jglobal;
      if (iglobal<first || iglobal>=last)
	continue;

      // diagonal element
      PetscScalar v = 4.0;
      PetscCall( MatSetValues(A,1,&iglobal,1,&iglobal,&v,INSERT_VALUES) ); 

      // previous row; we set an amount of unsymmetry if this is a periodic point
      jglobal = iglobal - domain_n; v = -1.0;
      if ( i>0 ) {
	PetscCall( MatSetValues(A,1,&iglobal,1,&jglobal,&v,INSERT_VALUES) ); 
      } else {
	jglobal += matrix_size; v = -unsymmetry;
	PetscCall( MatSetValues(A,1,&iglobal,1,&jglobal,&v,INSERT_VALUES) ); 
      }

      // next row; we set an amount of unsymmetry if this is a periodic point
      jglobal = iglobal + domain_n; v = -1.0;
      if ( i<domain_m-1 ) {
	PetscCall( MatSetValues(A,1,&iglobal,1,&jglobal,&v,INSERT_VALUES) ); 
      } else {
	jglobal -= matrix_size; v = +unsymmetry;
	PetscCall( MatSetValues(A,1,&iglobal,1,&jglobal,&v,INSERT_VALUES) ); 
      }

      // previous point; we set an amount of unsymmetry if this is a periodic point
      jglobal = iglobal - 1; v = -1.0;
      if ( j>0 )   {
	PetscCall( MatSetValues(A,1,&iglobal,1,&jglobal,&v,INSERT_VALUES) ); 
      } else {
	jglobal += domain_n; v = -unsymmetry;
	PetscCall( MatSetValues(A,1,&iglobal,1,&jglobal,&v,INSERT_VALUES) ); 
      }

      // next point; we set an amount of unsymmetry if this is a periodic point
      jglobal = iglobal + 1; v = -1.0;
      if ( j<domain_n-1 ) {
	PetscCall( MatSetValues(A,1,&iglobal,1,&jglobal,&v,INSERT_VALUES) ); 
      } else {
	jglobal -= domain_n; v = +unsymmetry;
	PetscCall( MatSetValues(A,1,&iglobal,1,&jglobal,&v,INSERT_VALUES) ); 
      }
    }
  }
  PetscCall( MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY) ); 
  PetscCall( MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY) ); 

  *rA = A;
  PetscFunctionReturn(0);
}
