/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "petsc.h"

int main(int argc,char **argv) {

  PetscErrorCode ierr;
  ierr = PetscInitialize(&argc,&argv,0,0); CHKERRQ(ierr);

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  int iprocs=1,jprocs;
  ierr = PetscOptionsGetInt(PETSC_NULL,PETSC_NULL,"-pi",&iprocs,PETSC_NULL); CHKERRQ(ierr);
  if (nprocs%iprocs!=0) {
    PetscPrintf(comm,"ip value %d does not divide nprocs=%d\n",iprocs,nprocs);
    iprocs = 1;
  }
  jprocs = nprocs/iprocs;
  PetscPrintf(comm,"Using processor grid %d x %d\n",iprocs,jprocs);

  int local_grid_size = 10;
  int global_elements_i = iprocs * local_grid_size,
    global_elements_j = jprocs * local_grid_size,
    global_nodes_i = global_elements_i+1,
    global_nodes_j = global_elements_j+1;

  DM dm;
  DMCreate(comm,&dm);
  DMSetType(dm,DMPLEX);
  DMSetDimension(dm,2);
  DMPlexSetChart(dm,0, global_elements_i*global_elements_j + global_nodes_i*global_nodes_j);

  PetscFinalize();
  return 0;
}
