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

static char help[] = "\nVector exercise. Commandline option: \"-n 123\" for problem size.\n\n";

#include <petscvec.h>

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **argv)
{
  Vec            x,y;               /* vectors */
  PetscInt       n = 200;
  PetscErrorCode ierr;

  PetscInitialize(&argc,&argv,(char*)0,help);
  MPI_Comm comm = PETSC_COMM_WORLD;

  /*
   * Get a commandline argument for the size of the problem
   */
  ierr = PetscOptionsGetInt
    (NULL,NULL,"-n",&n,NULL); CHKERRQ(ierr);

  /*
   * Create vector `x' with a default layout
   */
  ierr = VecCreate(comm,&x);CHKERRQ(ierr);
  ierr = VecSetSizes(x,PETSC_DECIDE,n);CHKERRQ(ierr);
  ierr = VecSetFromOptions(x);CHKERRQ(ierr);

  /*
   * Duplicate some work vectors (of the same format and
   * partitioning as the initial vector).
  */
  ierr = VecDuplicate(x,&y);CHKERRQ(ierr);

  /*
   * Set x,y values
  */
  PetscScalar    one = 1.0,two = 2.0;
  ierr = VecSet(x,one);CHKERRQ(ierr);
  {
    /*
     * Exercise 1:
     * -- Set y to a sine wave
     *    Find the correct bounds on the loop
     *    so that each process sets only local elements
     *    Set the correct index
     */
    PetscInt myfirst,mylast,localsize,globalsize;
    ierr = VecGetSize(y,&globalsize); CHKERRQ(ierr);
    ierr = VecGetLocalSize(y,&localsize); CHKERRQ(ierr);
    ierr = VecGetOwnershipRange(y,&myfirst,&mylast); CHKERRQ(ierr);
    for (PetscInt index=
/**** your code here ****/
	 index++) {
      PetscScalar value = sin( index * 2. * 3.14159 / globalsize );
      ierr = VecSetValue(y,
/**** your code here ****/
			 value,INSERT_VALUES); CHKERRQ(ierr);
    }
    ierr = VecAssemblyBegin(y); CHKERRQ(ierr);
    ierr = VecAssemblyEnd(y); CHKERRQ(ierr);
    //ierr = VecSet(y,two);CHKERRQ(ierr);
  }

  /*
   * Exercise 2:
   * - compute inner product of x and y
   */
  PetscScalar inprod;
/**** your code here ****/
  ierr = PetscPrintf
    (comm,"Computed inner product as %f, should be about zero\n",inprod); CHKERRQ(ierr);

  /* 
   * Exercise 3:
   * -- compute the norm of x
   * -- scale x down by that norm
   * -- check that the norm of the result is 1
   */
/**** your code here ****/
  PetscPrintf(comm,"Norm of scaled vector is %f, should be 1\n",norm);

  /*
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
  ierr = VecDestroy(&x);CHKERRQ(ierr);
  ierr = VecDestroy(&y);CHKERRQ(ierr);

  ierr = PetscFinalize();
  return 0;
}
