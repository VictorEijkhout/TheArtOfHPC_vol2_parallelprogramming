/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** grid1d.c : example of 1-dimensional smoothing
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

  PetscInt i_global = 10*nprocs;

  /*
   * Create a 2d grid and a matrix on that grid.
   */
  DM grid;
  ierr = DMDACreate1d            // IN:
    ( comm,                      // collective on this communicator
      DM_BOUNDARY_NONE,          // no periodicity and such
      i_global,                  // global size 100x100; can be changed with options
      1,                         // degree of freedom per node
      1,                         // stencil width
      NULL,                      // arrays of local sizes in each direction
      &grid                      // OUT: resulting object
      ); CHKERRQ(ierr);
  ierr = DMSetUp(grid); CHKERRQ(ierr);

  Mat A;
  ierr = DMCreateMatrix(grid,&A); CHKERRQ(ierr);

  /*
   * Print out how the grid is distributed over processors
   */
  PetscInt i_first,i_local;
  ierr = DMDAGetCorners(grid,&i_first,NULL,NULL,&i_local,NULL,NULL);CHKERRQ(ierr);
  /* ierr = PetscSynchronizedPrintf */
  /*   (comm, */
  /*    "[%d] Local part = %d-%d x %d-%d\n", */
  /*    procno,info.xs,info.xs+info.xm,info.ys,info.ys+info.ym); CHKERRQ(ierr); */
  /* ierr = PetscSynchronizedFlush(comm,stdout); CHKERRQ(ierr); */

  /*
   * Fill in the elements of the matrix
   */
  for (PetscInt i_index=i_first; i_index<i_first+i_local; i_index++) {
    MatStencil  row = {0},col[3] = {{0}};
    PetscScalar v[3];
    PetscInt    ncols = 0;
    row.i = i_index;
    col[ncols].i = i_index; v[ncols] = 2.;
    ncols++;
    if (i_index>0)          { col[ncols].i = i_index-1; v[ncols] = 1.; ncols++; }
    if (i_index<i_global-1) { col[ncols].i = i_index+1; v[ncols] = 1.; ncols++; }
    ierr = MatSetValuesStencil(A,1,&row,ncols,col,v,INSERT_VALUES);CHKERRQ(ierr);
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
   * Solve a linear system on the grid
   */
  KSP solver;
  ierr = KSPCreate(comm,&solver); CHKERRQ(ierr);
  ierr = KSPSetType(solver,KSPBCGS); CHKERRQ(ierr);
  ierr = KSPSetOperators(solver,A,A); CHKERRQ(ierr);
  ierr = KSPSetFromOptions(solver); CHKERRQ(ierr);
  ierr = KSPSolve(solver,x,y); CHKERRQ(ierr);

  /*
   * Report on success of the solver, or lack thereof
   */
  {
    PetscInt its; KSPConvergedReason reason; 
    ierr = KSPGetConvergedReason(solver,&reason);
    ierr = KSPGetIterationNumber(solver,&its); CHKERRQ(ierr);
    if (reason<0) {
      PetscPrintf(comm,"Failure to converge after %d iterations; reason %s\n",
		  its,KSPConvergedReasons[reason]);
    } else {
      PetscPrintf(comm,"Number of iterations to convergence: %d\n",its);
    }
  }

  /*
   * Clean up
   */
  ierr = KSPDestroy(&solver); CHKERRQ(ierr);
  ierr = VecDestroy(&x); CHKERRQ(ierr);
  ierr = VecDestroy(&y); CHKERRQ(ierr);
  ierr = MatDestroy(&A); CHKERRQ(ierr);
  ierr = DMDestroy(&grid); CHKERRQ(ierr);

  return PetscFinalize();
}
