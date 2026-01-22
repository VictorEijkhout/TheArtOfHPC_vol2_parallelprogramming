/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2024
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static char help[] = "\nMatrix exercise. Commandline option: \"-n 123\" for local problem size.\n\n";

#include <petscmat.h>

/*
 * Function to create the matrix.
 * Let's pretend that you do not know what happens here, 
 * so you have no information about the matrix size and such.
 */
PetscErrorCode CreateMatrix(MPI_Comm comm,int nlocal,Mat *rA) {

  Mat A;
  PetscInt first,last;
  PetscErrorCode ierr;
  int procno,nglobal;

  PetscCall( MatCreate(comm,&A) ); 
  PetscCall( MatSetType(A,MATMPIAIJ) ); 

  MPI_Comm_rank(comm,&procno);
  if (procno==0)
    nlocal = 1;
  PetscCall( MatSetSizes(A,nlocal,nlocal,PETSC_DECIDE,PETSC_DECIDE) ); 
  /*
   * Exercise
   * - what happens if you preallocate insufficient space?
   */
  PetscCall( MatMPIAIJSetPreallocation(A,3,NULL,1,NULL) ); 

  PetscCall( MatAssemblyBegin(A,MAT_FLUSH_ASSEMBLY) ); 
  PetscCall( MatAssemblyEnd(A,MAT_FLUSH_ASSEMBLY) ); 

  PetscCall( MatGetSize(A,&nglobal,PETSC_NULL) ); 
  PetscCall( MatGetOwnershipRange(A,&first,&last) ); 
  for (int row=first; row<last; row++) {
    PetscInt col = row; PetscScalar v = 2.;
    PetscCall( MatSetValue(A,row,col,v,INSERT_VALUES) ); 
    col = row-1; v = -1.;
    if (col>=0) {
      PetscCall( MatSetValue(A,row,col,v,INSERT_VALUES) ); 
    }
    col = row+1; v = -1.;
    if (col<nglobal) {
      PetscCall( MatSetValue(A,row,col,v,INSERT_VALUES) ); 
    }
  }
  PetscCall( MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY) ); 
  PetscCall( MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY) ); 

  *rA = A;
  return 0;
}

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **argv)
{
  Mat            A;
  Vec            x,y;               /* vectors */
  PetscInt       nlocal = 20;
  PetscErrorCode ierr;

  PetscInitialize(&argc,&argv,(char*)0,help);
  MPI_Comm comm = PETSC_COMM_WORLD;
  int procno,nprocs;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  /*
   * Get a commandline argument for the size of the problem
   */
  PetscCall( PetscOptionsGetInt(NULL,NULL,"-n",&nlocal,NULL) ); 

  /*
   * Call the matrix creation routine.
   * (Note the way we are treating the pointer to the matrix object.)
   */
  PetscCall( CreateMatrix(comm,nlocal,&A) ); 
  // just checking: screen output
  //MatView(A,PETSC_VIEWER_STDOUT_WORLD);

  /*
   * Start creating a vector
   */
  PetscCall( VecCreate(comm,&x) ); 
  PetscCall( VecSetType(x,VECMPI) ); 

  /*
   * Exercise:
   * -- find the local size of the matrix, and use that to set the vector size
   */
  {
    PetscInt rowsize;
    PetscCall( MatGetLocalSize
      (A,
       &rowsize,NULL
       ) );
    PetscInt globalsize;
    MatGetSize(A,&globalsize,NULL);
    PetscCall( VecSetSizes
      (x,
       PETSC_DECIDE,globalsize
       //rowsize,PETSC_DETERMINE
       ) );
  }

  /*
   * Duplicate some work vectors (of the same format and
   * partitioning as the initial vector).
  */
  PetscCall( VecDuplicate(x,&y) ); 

  /*
   * Set x to constant one
   */
  PetscScalar    one = 1.0;
  PetscCall( VecSet(x,one) ); 

  /*
   * Exercise:
   * -- fill in the correct parameters to compute 
   *    the matrix vector product y = Ax
   */
  PetscCall( MatMult
    (
     A,x,y
     ) );

  /*
   * First check on the product
   */
  double norm;
  PetscCall( VecNorm(y,NORM_1,&norm) ); 
  if ( fabs(2.-norm)>1.e-14) {
    PetscCall( PetscPrintf(comm,"Wrong norm: %e should be 2.\n",norm) ); 
  } else {
    PetscCall( PetscPrintf(comm,"Global norm test succeeds on all %d processes\n",nprocs) ); 
  }

  #if 0
  /*
   * Second, more funky test
   */
  {
    PetscScalar *localdata;
    Vec localvec;
    int localsize;
    double norm,norm_shouldbe;
    /*
     * Create a single-process vector with the size of the local part of y
     */
    PetscCall( VecCreate(PETSC_COMM_SELF,&localvec) ); 
    PetscCall( VecSetType(localvec,VECSEQ) ); 
    PetscCall( VecGetLocalSize(y,&localsize) ); 
    PetscCall( VecSetSizes(localvec,localsize,PETSC_DECIDE) ); 
    /*
     * Exercise:
     * -- put y's data into the local vector,
     *    and compute the local norm
     * -- hint: use VecGetArray to get the y data
     * --       use VecPlaceArray to set that in localvec
     */
/**** your code here ****/
    /*
     * Compute the norm: it should be 1 on the first & last process,
     * zero elsewhere
     */
    PetscCall( VecNorm(localvec,NORM_1,&norm) ); 
    if (procno==0 || procno==nprocs-1)
      norm_shouldbe = 1.;
    else norm_shouldbe = 0.;
    if ( fabs(norm-norm_shouldbe)>1.e-14 )
      printf("Wrong local norm %e on proc %d\n",norm,procno);
    else printf("Local norm succeeded\n");

    /*
     * Let's clean up
     */
    /*
     * Optional exercise:
     * -- what are the mirror routines of VecGetArray & VecPlaceArray above?
     *    (this mostly serves to prevent memory leaks)
     */
/**** your code here ****/
    PetscCall( VecDestroy(&localvec) ); 
  }
  #endif

  /*
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
  PetscCall( MatDestroy(&A) ); 
  PetscCall( VecDestroy(&x) ); 
  PetscCall( VecDestroy(&y) ); 

  PetscFinalize();
  return 0;
}
