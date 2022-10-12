/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
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

  ierr = MatCreate(comm,&A); CHKERRQ(ierr);
  ierr = MatSetType(A,MATMPIAIJ); CHKERRQ(ierr);

  MPI_Comm_rank(comm,&procno);
  if (procno==0)
    nlocal = 1;
  ierr = MatSetSizes(A,nlocal,nlocal,PETSC_DECIDE,PETSC_DECIDE); CHKERRQ(ierr);
  /*
   * Exercise
   * - what happens if you preallocate insufficient space?
   */
  ierr = MatMPIAIJSetPreallocation(A,3,NULL,1,NULL); CHKERRQ(ierr);

  ierr = MatAssemblyBegin(A,MAT_FLUSH_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatAssemblyEnd(A,MAT_FLUSH_ASSEMBLY); CHKERRQ(ierr);

  ierr = MatGetSize(A,&nglobal,PETSC_NULL); CHKERRQ(ierr);
  ierr = MatGetOwnershipRange(A,&first,&last); CHKERRQ(ierr);
  for (int row=first; row<last; row++) {
    PetscInt col = row; PetscScalar v = 2.;
    ierr = MatSetValue(A,row,col,v,INSERT_VALUES); CHKERRQ(ierr);
    col = row-1; v = -1.;
    if (col>=0) {
      ierr = MatSetValue(A,row,col,v,INSERT_VALUES); CHKERRQ(ierr);
    }
    col = row+1; v = -1.;
    if (col<nglobal) {
      ierr = MatSetValue(A,row,col,v,INSERT_VALUES); CHKERRQ(ierr);
    }
  }
  ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);

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
  ierr = PetscOptionsGetInt
    (NULL,NULL,"-n",&nlocal,NULL); CHKERRQ(ierr);

  /*
   * Call the matrix creation routine.
   * (Note the way we are treating the pointer to the matrix object.)
   */
  ierr = CreateMatrix(comm,nlocal,&A); CHKERRQ(ierr);
  // just checking: screen output
  //MatView(A,PETSC_VIEWER_STDOUT_WORLD);

  /*
   * Start creating a vector
   */
  ierr = VecCreate(comm,&x); CHKERRQ(ierr);
  ierr = VecSetType(x,VECMPI); CHKERRQ(ierr);

  /*
   * Exercise:
   * -- find the local size of the matrix, and use that to set the vector size
   */
  {
    PetscInt rowsize;
    ierr = MatGetLocalSize
      (A,
/**** your code here ****/
       ); CHKERRQ(ierr);
    ierr = VecSetSizes
      (x,
/**** your code here ****/
       ); CHKERRQ(ierr);
  }

  /*
   * Duplicate some work vectors (of the same format and
   * partitioning as the initial vector).
  */
  ierr = VecDuplicate(x,&y); CHKERRQ(ierr);

  /*
   * Set x to constant one
   */
  PetscScalar    one = 1.0;
  ierr = VecSet(x,one); CHKERRQ(ierr);

  /*
   * Exercise:
   * -- fill in the correct parameters to compute 
   *    the matrix vector product y = Ax
   */
  ierr = MatMult
    (
/**** your code here ****/
     ); CHKERRQ(ierr);

  /*
   * First check on the product
   */
  double norm;
  ierr = VecNorm(y,NORM_1,&norm); CHKERRQ(ierr);
  if ( fabs(2.-norm)>1.e-14) {
    ierr = PetscPrintf(comm,"Wrong norm: %e should be 2.\n",norm); CHKERRQ(ierr);
  } else {
    ierr = PetscPrintf(comm,
	   "Global norm test succeeds on all %d processes\n",nprocs); CHKERRQ(ierr);
  }

  //#if 0
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
    ierr = VecCreate(PETSC_COMM_SELF,&localvec); CHKERRQ(ierr);
    ierr = VecSetType(localvec,VECSEQ); CHKERRQ(ierr);
    ierr = VecGetLocalSize(y,&localsize); CHKERRQ(ierr);
    ierr = VecSetSizes(localvec,localsize,PETSC_DECIDE); CHKERRQ(ierr);
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
    ierr = VecNorm(localvec,NORM_1,&norm); CHKERRQ(ierr);
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
    ierr = VecDestroy(&localvec); CHKERRQ(ierr);
  }
  //#endif

  /*
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
  ierr = MatDestroy(&A); CHKERRQ(ierr);
  ierr = VecDestroy(&x); CHKERRQ(ierr);
  ierr = VecDestroy(&y); CHKERRQ(ierr);

  ierr = PetscFinalize();
  return 0;
}
