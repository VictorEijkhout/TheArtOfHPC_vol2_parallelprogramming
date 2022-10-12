/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** root.c : exercise for backtracing error messages
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <petsc.h>

PetscErrorCode square_root(PetscReal x,PetscReal *rootx) {
  PetscFunctionBegin;
  /*
   * Exercise:
   * set the `rootx' variable for nonnegative input,
   * or return an error for negative input.
   */
/**** your code here ****/
  PetscFunctionReturn(0);
}

int main(int argc,char **argv)
{
  PetscErrorCode ierr;

  char help[] = "\nInit example.\n\n";
  ierr = PetscInitialize(&argc,&argv,(char*)0,help); CHKERRQ(ierr);
  PetscReal x,rootx;
  x = 1.5; ierr = square_root(x,&rootx); CHKERRQ(ierr);
  PetscPrintf(PETSC_COMM_WORLD,"Root of %f is %f\n",x,rootx);
  x = -2.6; ierr = square_root(x,&rootx); CHKERRQ(ierr);
  PetscPrintf(PETSC_COMM_WORLD,"Root of %f is %f\n",x,rootx);
  ierr = PetscFinalize(); CHKERRQ(ierr);
  return 0;
}
