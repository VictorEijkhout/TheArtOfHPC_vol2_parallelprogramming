/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** 
 **** transpose.c : transposing is a basic block of FFT
 ****
 ****************************************************************/

#include "petscksp.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  int dom_size,matrix_size;
  PetscErrorCode ierr;
  PetscBool flag;
  MPI_Comm comm, self;
  KSP Solve;
  Mat A;
  Vec Rhs,Sol;
  int localsize;
  PetscScalar one = 1.0;
  PetscScalar u=.1;
  
  PetscFunctionBegin;
  PetscInitialize(&argc,&args,0,0);

  comm = MPI_COMM_WORLD; self = MPI_COMM_SELF;
  int nprocs,procid;
  MPI_Comm_rank(comm,&procid);
  MPI_Comm_size(comm,&nprocs);

  PetscInt
    i_factor = 1,
    j_factor = 1;
  ierr = PetscOptionsGetInt(PETSC_NULL,PETSC_NULL,"-i",&i_factor,NULL); CHKERRQ(ierr);
  ierr = PetscOptionsGetInt(PETSC_NULL,PETSC_NULL,"-j",&j_factor,NULL); CHKERRQ(ierr);
  PetscInt
    i_size = nprocs * i_factor,
    j_size = nprocs * j_factor,
    Nglobal = i_size * j_size;
  PetscInt Nlocal = Nglobal/nprocs;
  if (Nglobal%nprocs!=0) {
    printf("Global size %d should be multiple of P=%d\n",Nglobal,nprocs);
    PetscFunctionReturn(-1); }

  Vec i_local;
  ierr = VecCreate(comm,&i_local); CHKERRQ(ierr);
  ierr = VecSetType(i_local,VECMPI); CHKERRQ(ierr);
  ierr = VecSetSizes(i_local,PETSC_DECIDE /*i_size*/,Nglobal); CHKERRQ(ierr);

  Vec j_local;
  ierr = VecCreate(comm,&j_local); CHKERRQ(ierr);
  ierr = VecSetType(j_local,VECMPI); CHKERRQ(ierr);
  ierr = VecSetSizes(j_local,PETSC_DECIDE /*j_size*/,Nglobal); CHKERRQ(ierr);

  /*
   * Dummy test data: lexicographic by columns
   */
  {
    PetscInt myfirst,mylast;
    ierr = VecGetOwnershipRange(i_local,&myfirst,&mylast); CHKERRQ(ierr);
    for (PetscInt index=myfirst; index<mylast; index++) {
      PetscScalar v = index;
      ierr = VecSetValue(i_local,index,v,INSERT_VALUES); CHKERRQ(ierr);
    }
    ierr = VecAssemblyBegin(i_local); CHKERRQ(ierr);
    ierr = VecAssemblyEnd(i_local); CHKERRQ(ierr);
  }
  VecScatter transpose_forward;
  {
    PetscInt
      i_chunk = i_size / nprocs,
      j_chunk = j_size / nprocs;
    if (i_chunk==0) {
      PetscPrintf(comm,"i chunk is zero\n"); PetscFunctionReturn(0); }
    if (j_chunk==0) {
      PetscPrintf(comm,"j chunk is zero\n"); PetscFunctionReturn(0); }
    if (i_size%nprocs!=0) {
      PetscPrintf(comm,"i size not divisible\n"); PetscFunctionReturn(0); }
    if (j_size%nprocs!=0) {
      PetscPrintf(comm,"j size not divisible\n"); PetscFunctionReturn(0); }

    IS space_to_get;
    PetscInt *indices_to_get;
    ierr = PetscMalloc1(Nlocal,&indices_to_get); CHKERRQ(ierr);
    for (PetscInt line=procid*i_chunk,write_loc=0; line<(procid+1)*i_chunk; line++) {
      for (PetscInt icol=0; icol<j_size; icol++) {
	PetscInt col = icol*i_size;
	indices_to_get[write_loc++] = line + col;
      }
    }
    ierr = ISCreateGeneral
      (comm,Nlocal,indices_to_get,PETSC_COPY_VALUES,&space_to_get); CHKERRQ(ierr);
    ierr = PetscFree(indices_to_get); CHKERRQ(ierr);
    ISView(space_to_get,0);
    ierr = VecScatterCreate
      (i_local,space_to_get,j_local,NULL,/* space_to_set, */ 
       &transpose_forward); CHKERRQ(ierr);
    ierr = ISDestroy(&space_to_get); CHKERRQ(ierr);
  }

  ierr = VecScatterBegin
      (transpose_forward,i_local,j_local,INSERT_VALUES,SCATTER_FORWARD); CHKERRQ(ierr);
  ierr = VecScatterEnd
    (transpose_forward,i_local,j_local,INSERT_VALUES,SCATTER_FORWARD); CHKERRQ(ierr);
  ierr = VecView(i_local,0); CHKERRQ(ierr);
  ierr = VecView(j_local,0); CHKERRQ(ierr);

  ierr = VecScatterDestroy(&transpose_forward); CHKERRQ(ierr);
  ierr = VecDestroy(&i_local); CHKERRQ(ierr);
  ierr = VecDestroy(&j_local); CHKERRQ(ierr);

  return PetscFinalize();
}

