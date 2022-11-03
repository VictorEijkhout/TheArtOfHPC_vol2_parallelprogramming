/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** optionsbegin.c : explore options processing
 ****
 ****************************************************************/

#include "petsc.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  PetscErrorCode ierr;
  MPI_Comm comm;
  
  PetscFunctionBegin;
  PetscInitialize(&argc,&args,0,0);
  comm = MPI_COMM_WORLD;

  PetscInt i_value = 1, j_value = 1;
  PetscBool i_flag = 0, j_flag = 0;

  ierr = PetscOptionsBegin(comm,NULL,"Parameters",NULL); CHKERRQ(ierr);
  ierr = PetscOptionsInt("-i","i value",__FILE__,i_value,&i_value,&i_flag); CHKERRQ(ierr);
  ierr = PetscOptionsInt("-j","j value",__FILE__,j_value,&j_value,&j_flag); CHKERRQ(ierr);
  ierr = PetscOptionsEnd(); CHKERRQ(ierr);
  if (i_flag)
    PetscPrintf(comm,"Option `-i' was used\n");
  if (j_flag)
    PetscPrintf(comm,"Option `-j' was used\n");

  PetscPrintf(comm,"i=%d, j=%d\n",i_value,j_value);

  return PetscFinalize();
}

