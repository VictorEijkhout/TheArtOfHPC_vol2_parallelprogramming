/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** init.c : petsc initialization
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <petscsys.h>

int main(int argc,char **argv)
{
  PetscErrorCode ierr;

  char help[] = "\nInit example.\n\n";
  ierr = PetscInitialize
    (&argc,&argv,(char*)0,help); CHKERRQ(ierr);
  int flag;
  MPI_Initialized(&flag);
  if (flag)
    printf("MPI was initialized by PETSc\n");
  else
    printf("MPI not yet initialized\n");

  return PetscFinalize();
}
