/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 ****************************************************************/

#include <stdio.h>
#include "petsc.h"

int main(int Argc,char **Args)
{
  int ierr,procno,nprocs;

  ierr = PetscInitialize(&Argc,&Args,PETSC_NULL,PETSC_NULL); CHKERRQ(ierr);
  MPI_Comm comm = PETSC_COMM_WORLD;
  
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  /*
   * Exercise 1:
   * -- use PetscPrintf to print only from process zero
   *    make sure to remove the original printf!
   */
  printf(
	 "Hello, I'm processor %d and %d processors have just initialized\n",
	 procno,nprocs
	 );
  ierr = PetscPrintf
    (
/**** your code here ****/
     ); CHKERRQ(ierr);

  /*
   * Exercise 2:
   * -- use PetscSynchronizedPrintf to let each processor print in sequence
   */
#if 0
  ierr = PetscSynchronizedPrintf
    (
/**** your code here ****/
     ); CHKERRQ(ierr);
  ierr = PetscSynchronizedFlush(comm,stdout); CHKERRQ(ierr);
#endif

  ierr = PetscFinalize(); CHKERRQ(ierr);

  return 0;
}
