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
   * Get a commandline argument for the local size of the problem
   */
  n = 1;
  ierr = PetscOptionsGetInt
    (NULL,NULL,"-n",&n,NULL); CHKERRQ(ierr);

  /*
   * Create vector `x' with a default layout
   */
  ierr = VecCreate(comm,&x);CHKERRQ(ierr);
  ierr = VecSetSizes(x,n,PETSC_DECIDE);CHKERRQ(ierr);
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

  /*
   * Get arrays and operate on them directly
   */
  {
    PetscScalar const *in_array;
    PetscScalar *out_array;
    VecGetArrayRead(x,&in_array);
    VecGetArray(y,&out_array);
    PetscInt localsize;
    VecGetLocalSize(x,&localsize);
    for (int i=0; i<localsize; i++)
      out_array[i] = 2*in_array[i];
    VecRestoreArrayRead(x,&in_array);
    VecRestoreArray(y,&out_array);
  }

  /* 
   * Test the result
   */
  PetscReal xnorm,ynorm;
  ierr = VecNorm(x,NORM_2,&xnorm); CHKERRQ(ierr);
  ierr = VecNorm(x,NORM_2,&ynorm); CHKERRQ(ierr);
  PetscPrintf(comm,"Norms x: %e, y: %e\n",xnorm,ynorm);

  /*
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
  ierr = VecDestroy(&x);CHKERRQ(ierr);
  ierr = VecDestroy(&y);CHKERRQ(ierr);

  ierr = PetscFinalize();
  return 0;
}
