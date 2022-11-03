/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** twoksps.c : NOT A REAL EXXAMPLE OF ANYTHING
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <petsc.h>

int main(int argc,char **argv) {
  PetscErrorCode ierr;
  PetscFunctionBegin;
  ierr = PetscInitialize(&argc,&argv,0,0); CHKERRQ(ierr);
  MPI_Comm comm = PETSC_COMM_WORLD;

  Mat scalar;
  ierr = MatCreate(comm,&scalar); CHKERRQ(ierr);
  ierr = MatSetType(scalar,MATSEQAIJ); CHKERRQ(ierr);
  ierr = MatSetSizes(scalar,1,1,1,1); CHKERRQ(ierr);
  PetscScalar v = 1.;
  ierr = MatSetUp(scalar); CHKERRQ(ierr);
  ierr = MatSetValue(scalar,0,0,v,INSERT_VALUES); CHKERRQ(ierr);
  ierr = MatAssemblyBegin(scalar,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatAssemblyEnd(scalar,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);

  Vec in,out;
  ierr = VecCreate(comm,&in); CHKERRQ(ierr);
  ierr = VecSetType(in,VECSEQ); CHKERRQ(ierr);
  ierr = VecSetSizes(in,1,1); CHKERRQ(ierr);
  ierr = VecSetValue(in,0,1.,INSERT_VALUES); CHKERRQ(ierr);
  ierr = VecAssemblyBegin(in); CHKERRQ(ierr);
  ierr = VecAssemblyEnd(in); CHKERRQ(ierr);
  ierr = VecDuplicate(in,&out); CHKERRQ(ierr);

  KSP time,space;
  ierr = KSPCreate(comm,&time); CHKERRQ(ierr);
  ierr = KSPSetType(time,KSPCG); CHKERRQ(ierr);
  ierr = KSPSetOperators(time,scalar,scalar); CHKERRQ(ierr);
  //  ierr = KSPSetOptionsPrefix(time,"time_"); CHKERRQ(ierr);
  ierr = KSPSetFromOptions(time); CHKERRQ(ierr);
  ierr = KSPSolve(time,in,out); CHKERRQ(ierr);

  return PetscFinalize();
}
