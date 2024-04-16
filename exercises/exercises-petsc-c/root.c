/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2024
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
  PetscCall( PetscInitialize(&argc,&argv,(char*)0,help) ); 
  PetscReal x,rootx;
  x = 1.5; PetscCall( square_root(x,&rootx) ); 
  PetscPrintf(PETSC_COMM_WORLD,"Root of %f is %f\n",x,rootx);
  x = -2.6; PetscCall( square_root(x,&rootx) ); 
  PetscPrintf(PETSC_COMM_WORLD,"Root of %f is %f\n",x,rootx);
  PetscCall( PetscFinalize() ); 
  return 0;
}
