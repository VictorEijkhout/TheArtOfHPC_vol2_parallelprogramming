/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2022
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "petsc.h"
#include "petscdmda.h"

int main(int argc,char **argv) {

  PetscErrorCode ierr;
  ierr = PetscInitialize(&argc,&argv,0,0); CHKERRQ(ierr);

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  /* if (nprocs%2!=0) { */
  /*   SETERRQ(comm,1,"Need even number of processors\n"); */
  /* } */

  /*
   * Create a 2d grid and a matrix on that grid.
   */
  DM grid;
  ierr = DMDACreate2d            // IN:
    ( comm,                      // collective on this communicator
      DM_BOUNDARY_NONE,DM_BOUNDARY_NONE, // no periodicity and such
      DMDA_STENCIL_STAR,         // no cross connections
      100,100,                 // global size 100x100; can be changed with options
      PETSC_DECIDE,PETSC_DECIDE, // processors in each direction
      1,                         // degree of freedom per node
      1,                         // stencil width
      NULL,NULL,                 // arrays of local sizes in each direction
      &grid                      // OUT: resulting object
      ); CHKERRQ(ierr);
  ierr = DMSetUp(grid); CHKERRQ(ierr);

  Mat A;
  ierr = DMCreateMatrix(grid,&A); CHKERRQ(ierr);

  /*
   * Print out how the grid is distributed over processors
   */
  DMDALocalInfo  info;
  ierr = DMDAGetLocalInfo(grid,&info);CHKERRQ(ierr);
  ierr = PetscSynchronizedPrintf
    (comm,
     "[%d] Local part = %d-%d x %d-%d\n",
     procno,info.xs,info.xs+info.xm,info.ys,info.ys+info.ym); CHKERRQ(ierr);
  ierr = PetscSynchronizedFlush(comm,stdout); CHKERRQ(ierr);

  /*
   * Fill in the elements of the matrix
   *
   * Bonus Exercise:
   * -- this is code for filling in a 5-point matrix.
   *    Commented out is code for making it a 9-point matrix. Add it.
   * -- If you run the augmented code you'll get a runtime error.
   *    Take another look at the DMDACreate2d call and fix one parameter.
   */
  for (int j=info.ys; j<info.ys+info.ym; j++) {
    for (int i=info.xs; i<info.xs+info.xm; i++) {
      MatStencil  row,col[5];
      PetscScalar v[5];
      PetscInt    ncols = 0;
      row.j        = j; row.i = i;
      /**** local connection: diagonal element ****/
      col[ncols].j = j; col[ncols].i = i; v[ncols++] = 4.;
      /* boundaries: top and bottom row */
      if (i>0)         {col[ncols].j = j;   col[ncols].i = i-1; v[ncols++] = -1.;}
      if (i<info.mx-1) {col[ncols].j = j;   col[ncols].i = i+1; v[ncols++] = -1.;}
      /* boundary left and right */
      if (j>0)         {col[ncols].j = j-1; col[ncols].i = i;   v[ncols++] = -1.;}
      if (j<info.my-1) {col[ncols].j = j+1; col[ncols].i = i;   v[ncols++] = -1.;}

      ierr = MatSetValuesStencil(A,1,&row,ncols,col,v,INSERT_VALUES);CHKERRQ(ierr);
    }
  }
  ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);

  /*
   * Create vectors on the grid
   */
  Vec x,y;
  ierr = DMCreateGlobalVector(grid,&x); CHKERRQ(ierr);
  ierr = VecDuplicate(x,&y); CHKERRQ(ierr);

  /*
   * Set vector values: first locally, then global
   */
  PetscReal one = 1.;
  {
    Vec xlocal;
    ierr = DMCreateLocalVector(grid,&xlocal); CHKERRQ(ierr);
    ierr = VecSet(xlocal,one); CHKERRQ(ierr);
    ierr = DMLocalToGlobalBegin(grid,xlocal,INSERT_VALUES,x); CHKERRQ(ierr);
    ierr = DMLocalToGlobalEnd(grid,xlocal,INSERT_VALUES,x); CHKERRQ(ierr);
    ierr = VecDestroy(&xlocal); CHKERRQ(ierr);
  }

  /*
   * Clean up
   */
  ierr = VecDestroy(&x); CHKERRQ(ierr);
  ierr = VecDestroy(&y); CHKERRQ(ierr);
  ierr = MatDestroy(&A); CHKERRQ(ierr);
  ierr = DMDestroy(&grid); CHKERRQ(ierr);

  PetscFinalize();
  return 0;
}
