/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** size.c : size of petsc quantities
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <petscsys.h>

int main(int argc,char **argv)
{
  PetscErrorCode ierr;
  char help[] = "\nSizse measurements.\n\n";
  ierr = PetscInitialize(&argc,&argv,(char*)0,help); CHKERRQ(ierr);

  printf("size of PetscInt : %lu\n",sizeof(PetscInt));
  printf("size of MPI Int  : %lu\n",sizeof(PetscMPIInt));
  printf("size of PetscReal: %lu\n",sizeof(PetscReal));

  return PetscFinalize();
}
