/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** average.c : repeated application of an averaging.
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static char help[] = "\nFFT example.\n\n";

#include <petscmat.h>

int main(int argc,char **argv)
{
  Vec            x,y;               /* vectors */
  int nprocs,procid;
  PetscErrorCode ierr;

  PetscInitialize(&argc,&argv,(char*)0,help);
  MPI_Comm comm = PETSC_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procid);
  
  /*
   * Set problem parameters
   */
  PetscInt lines_per_proc = 10;
  ierr = PetscOptionsGetInt
    (PETSC_NULL,PETSC_NULL,"-nlines",&lines_per_proc,NULL); CHKERRQ(ierr);
  PetscInt Nlines = lines_per_proc * nprocs, LineLength = lines_per_proc * nprocs,
    Nglobal = Nlines * LineLength;
  ierr = PetscPrintf(comm,"Using global size %d\n",Nglobal); CHKERRQ(ierr);
  PetscInt maxit = 100;
  ierr = PetscOptionsGetInt
    (PETSC_NULL,PETSC_NULL,"-maxit",&maxit,NULL); CHKERRQ(ierr);
  ierr = PetscPrintf(comm,"Running for %d iterations\n",Nglobal); CHKERRQ(ierr);

  /*
   * Create matrix object
   */
  Mat average;
  ierr = MatCreate(comm,&average); CHKERRQ(ierr);
  ierr = MatSetType(average,MATMPIAIJ); CHKERRQ(ierr);
  ierr = MatSetSizes(average,PETSC_DECIDE,PETSC_DECIDE,Nglobal,Nglobal); CHKERRQ(ierr);
  ierr = MatMPIAIJSetPreallocation(average,5,NULL,2,NULL); CHKERRQ(ierr);

  PetscInt myfirst,mylast;
  ierr = MatGetOwnershipRange(average,&myfirst,&mylast); CHKERRQ(ierr);
  for (PetscInt index=myfirst; index<mylast; index++) {
    PetscInt irow = index/Nlines, icol = index % Nlines;
    PetscScalar four = 4., one = -1.;
    PetscInt Iglobal = index, Jglobal = index;
    ierr = MatSetValue(average,Iglobal,Jglobal,four,INSERT_VALUES); CHKERRQ(ierr);
    if (Iglobal>0) {
      Jglobal = Iglobal - LineLength;
      ierr = MatSetValue(average,Iglobal,Jglobal,one,INSERT_VALUES); CHKERRQ(ierr);
    }
    if (Iglobal<Nlines-1) {
      Jglobal = Iglobal + LineLength;
      ierr = MatSetValue(average,Iglobal,Jglobal,one,INSERT_VALUES); CHKERRQ(ierr);
    }
    if (Jglobal>0) {
      Jglobal = Iglobal - 1;
      ierr = MatSetValue(average,Iglobal,Jglobal,one,INSERT_VALUES); CHKERRQ(ierr);
    }
    if (Jglobal<LineLength-1) {
      Jglobal = Iglobal + 1;
      ierr = MatSetValue(average,Iglobal,Jglobal,one,INSERT_VALUES); CHKERRQ(ierr);
    }
  }
  ierr = MatAssemblyBegin(average,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatAssemblyEnd(average,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);

  ierr = VecCreate(comm,&x); CHKERRQ(ierr);
  ierr = VecSetType(x,VECMPI); CHKERRQ(ierr);
  ierr = VecSetSizes(x,PETSC_DECIDE,Nglobal); CHKERRQ(ierr);
  ierr = VecDuplicate(x,&y); CHKERRQ(ierr);

  PetscLogDouble tstart,tend,duration;
  ierr = PetscTime(&tstart); CHKERRQ(ierr);
  for (int iter=0; iter<maxit/2; iter++) {
    ierr = MatMult(average,x,y); CHKERRQ(ierr);
    ierr = MatMult(average,y,x); CHKERRQ(ierr);
  }
  ierr = PetscTime(&tend); CHKERRQ(ierr);
  duration = tend-tstart;
  ierr = PetscPrintf(comm,"Time for %d iterations: %e\n",maxit,duration); CHKERRQ(ierr);

  ierr = MatDestroy(&average); CHKERRQ(ierr);
  ierr = VecDestroy(&x); CHKERRQ(ierr);
  ierr = VecDestroy(&y); CHKERRQ(ierr);

  return PetscFinalize();
}
