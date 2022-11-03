#include <stdlib.h>
#include <stdio.h>
#include <petsc.h>

int main(int argc,char **argv) {
  PetscErrorCode ierr;
  PetscFunctionBegin;
  ierr = PetscInitialize(&argc,&argv,0,0); CHKERRQ(ierr);
  ierr = PetscFinalize(); CHKERRQ(ierr);
  return PetscFinalize();
}
