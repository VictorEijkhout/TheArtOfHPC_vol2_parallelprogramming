/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** fft.c : demonstrate MatFFT
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
  
  PetscInt Nlocal = 10, Nglobal = Nlocal*nprocs;
  PetscPrintf(comm,"FFT examples on N=%d n=%d\n",Nglobal,Nlocal);

  Mat transform;
  int dimensionality=1;
  PetscInt dimensions[dimensionality]; dimensions[0] = Nglobal; 
  PetscPrintf(comm,"Creating fft D=%d, dim=%d\n",dimensionality,dimensions[0]);
  ierr = MatCreateFFT(comm,dimensionality,dimensions,MATFFTW,&transform); CHKERRQ(ierr);
  {
    PetscInt fft_i,fft_j;
    ierr = MatGetSize(transform,&fft_i,&fft_j); CHKERRQ(ierr);
    PetscPrintf(comm,"FFT global size %d x %d\n",fft_i,fft_j);
  }
  Vec signal,frequencies;
  ierr = MatCreateVecsFFTW(transform,&frequencies,&signal,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject)signal,"signal"); CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject)frequencies,"frequencies"); CHKERRQ(ierr);
  ierr = VecAssemblyBegin(signal); CHKERRQ(ierr);
  ierr = VecAssemblyEnd(signal); CHKERRQ(ierr);
  {
    PetscInt nlocal,nglobal;
    ierr = VecGetLocalSize(signal,&nlocal); CHKERRQ(ierr);
    ierr = VecGetSize(signal,&nglobal); CHKERRQ(ierr);
    ierr = PetscPrintf(comm,"Signal local=%d global=%d\n",nlocal,nglobal); CHKERRQ(ierr);
  }

  PetscInt myfirst,mylast;
  ierr = VecGetOwnershipRange(signal,&myfirst,&mylast); CHKERRQ(ierr);
  printf("Setting %d -- %d\n",myfirst,mylast);
  for (PetscInt vecindex=0; vecindex<Nglobal; vecindex++) {
    PetscScalar
      pi = 4. * atan(1.0),
      h = 1./Nglobal,
      phi = 2* pi * vecindex * h, 
      puresine = cos( phi )
#if defined(PETSC_USE_COMPLEX)
          + PETSC_i * sin(phi)
#endif
      ;
    ierr = VecSetValue(signal,vecindex,puresine,INSERT_VALUES); CHKERRQ(ierr);
  }
  ierr = VecAssemblyBegin(signal); CHKERRQ(ierr);
  ierr = VecAssemblyEnd(signal); CHKERRQ(ierr);

  ierr = VecView(signal,PETSC_VIEWER_STDOUT_WORLD); CHKERRQ(ierr);
  ierr = MatMult(transform,signal,frequencies); CHKERRQ(ierr);
  ierr = VecScale(frequencies,1./Nglobal); CHKERRQ(ierr);
  ierr = VecView(frequencies,PETSC_VIEWER_STDOUT_WORLD); CHKERRQ(ierr);

  {
    Vec confirm;
    ierr = VecDuplicate(signal,&confirm); CHKERRQ(ierr);
    ierr = MatMultTranspose(transform,frequencies,confirm); CHKERRQ(ierr);
    ierr = VecAXPY(confirm,-1,signal); CHKERRQ(ierr);
    PetscReal nrm;
    ierr = VecNorm(confirm,NORM_2,&nrm); CHKERRQ(ierr);
    PetscPrintf(MPI_COMM_WORLD,"FFT accuracy %e\n",nrm);
    ierr = VecDestroy(&confirm); CHKERRQ(ierr);
  }

  ierr = MatDestroy(&transform); CHKERRQ(ierr);
  ierr = VecDestroy(&signal); CHKERRQ(ierr);
  ierr = VecDestroy(&frequencies); CHKERRQ(ierr);

  return PetscFinalize();
}
