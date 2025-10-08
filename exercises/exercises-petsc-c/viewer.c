/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2024
 ****
 ****************************************************************/

#include "petscmat.h"
#include <stdio.h>

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  PetscViewer    fd;
  Mat            A; 
  PetscInt       i, j, n;
  int nprocs; 
  PetscErrorCode ierr;
  PetscBool      flag;
  PetscScalar    v, one = 1.0;
  Vec            u, b;

  PetscFunctionBegin;
  PetscInitialize(&argc,&args,NULL,NULL);

  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
  if (nprocs>1) SETERRQ(MPI_COMM_WORLD,1,"This is a sequential example");

  /*
   * Write or read the matrix
   */
  PetscCall( PetscOptionsGetInt(NULL,NULL,"-n",&n,&flag) ); 
  if (flag) {
    /*
     * If the user supplies a size flag: "-n 123", a new matrix 
     * file is created through a binary viewer.
     *
     * First we create a matrix.
     */
    PetscCall( MatCreateSeqAIJ(MPI_COMM_WORLD,n,n,n,0,&A) ); 
    for (i=0; i<n; i++)
      for (j=0; j<n; j++) {
	v = 1./(i+j+1);
	PetscCall( MatSetValues(A,1,&i,1,&j,&v,INSERT_VALUES) ); 
      }
    PetscCall( MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY) ); 
    PetscCall( MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY) ); 

    /*
     * Exercise:
     * - open a viewer for writing: find the Viewer routine for binary
     */
    // supply the right routine name:
/**** your code here ****/
      ( MPI_COMM_WORLD,"matdata",
       // find the right mode:
/**** your code here ****/
	&fd) );
    // now use the viewer object to 'view' the matrix
    PetscCall( MatView(A,fd) );
    // for cleanliness, delete the viewer again
/**** your code here ****/

  } else {
    /*
     * Without the "-n" flag, you open an existing file and load it in.
     * This again uses a viewer.
     *
     * Exercise:
     * - supply the viewer call that opens a binary file for reading
     */
    // supply the right routine name:
/**** your code here ****/
      ( MPI_COMM_WORLD,"matdata",
	// find the right mode:
/**** your code here ****/
	&fd) );
    PetscCall( MatCreate(MPI_COMM_WORLD,&A) ); 
    PetscCall( MatSetType(A,MATSEQAIJ) ); 

    /*
     * Exercise:
     * - find the routine for loading a matrix from a binary viewer and use it here
     */
    // supply the correct routine for matrix loading
/**** your code here ****/
    PetscCall( PetscViewerDestroy(&fd) );

    /*
     * To ensure that we read the matrix correctly, we print the size.
     *  This should match the "-n" flag used to create the binary file.
     */
    PetscCall( MatGetSize(A,&n,NULL) ); 
    PetscCall( PetscPrintf(MPI_COMM_WORLD,"Read matrix of size %d\n",n) );
  }

  PetscCall( MatDestroy(&A) ); 

  PetscFinalize();
  PetscFunctionReturn(0);
}
