/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2024
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "petsc.h"
#include "petscdmda.h"

int main(int argc,char **argv) {

  PetscErrorCode ierr;
  PetscCall( PetscInitialize(&argc,&argv,0,0) ); 

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  if (nprocs%2!=0) {
    SETERRQ(comm,1,"Need even number of processors\n");
  }

  /*
   * Create a 2d grid and a matrix on that grid.
   */
  DM grid;
  PetscCall( DMDACreate2d            // IN:
    ( comm,                      // collective on this communicator
      DM_BOUNDARY_NONE,DM_BOUNDARY_NONE, // no periodicity and such
      DMDA_STENCIL_STAR,         // no cross connections
      100,100,                 // global size 100x100; can be changed with options
      PETSC_DECIDE,PETSC_DECIDE, // processors in each direction
      1,                         // degree of freedom per node
      1,                         // stencil width
      NULL,NULL,                 // arrays of local sizes in each direction
      &grid                      // OUT: resulting object
      ) );
  PetscCall( DMSetUp(grid) ); 

  Mat A;
  PetscCall( DMCreateMatrix(grid,&A) ); 

  /*
   * Print out how the grid is distributed over processors
   */
  DMDALocalInfo  info;
  PetscCall( DMDAGetLocalInfo(grid,&info) );
  PetscCall( PetscSynchronizedPrintf
    (comm,
     "[%d] Local part = %d-%d x %d-%d\n",
     procno,info.xs,info.xs+info.xm,info.ys,info.ys+info.ym) );
  PetscCall( PetscSynchronizedFlush(comm,stdout) ); 

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
      MatStencil  row = {0},col[
				5 // number of columns, change for 9-point stencil
/**** your code here ****/
				] = {{0}};
      PetscScalar v[
		    5 // number of columns: change for 9-point stencil
/**** your code here ****/
		    ];
      PetscInt    ncols = 0;
      row.j        = j; row.i = i;
      /**** local connection: diagonal element ****/
      col[ncols].j = j; col[ncols].i = i; v[ncols++] = 4.;/**** Make it 8 with cross points ****/

      /* boundaries: top row */
      if (i>0)         {col[ncols].j = j;   col[ncols].i = i-1; v[ncols++] = -1.;}
      /**** Cross points commented out for now ****/
      /*
      if (i>0 && j>0)  {col[ncols].j = j-1; col[ncols].i = i-1; v[ncols++] = -1.;}
      if (i>0 && j<info.my-1)
	               {col[ncols].j = j+1; col[ncols].i = i-1; v[ncols++] = -1.;}
      */

      /* boundary left and right */
      if (j>0)         {col[ncols].j = j-1; col[ncols].i = i;   v[ncols++] = -1.;}
      if (j<info.my-1) {col[ncols].j = j+1; col[ncols].i = i;   v[ncols++] = -1.;}

      /* boundary: bottom row */
      if (i<info.mx-1) {col[ncols].j = j;   col[ncols].i = i+1; v[ncols++] = -1.;}
      /**** Cross points commented out for now ****/
      /*
      if (i<info.mx-1 && j>0)
	               {col[ncols].j = j-1; col[ncols].i = i+1; v[ncols++] = -1.;}
      if (i<info.mx-1 && j<info.my-1)
	               {col[ncols].j = j+1; col[ncols].i = i+1; v[ncols++] = -1.;}
      */
      PetscCall( MatSetValuesStencil(A,1,&row,ncols,col,v,INSERT_VALUES) );
    }
  }
  /*
   * Exercise:
   * -- there are two lines missing here. Run your code and look at the runtime error.
   *    Can you figure out what calls to insert?
   */
/**** your code here ****/

  /*
   * Create vectors on the grid
   */
  Vec x,y;
  PetscCall( DMCreateGlobalVector(grid,&x) ); 
  PetscCall( VecDuplicate(x,&y) ); 

  /*
   * Set vector values: first locally, then global
   */
  PetscReal one = 1.;
  {
    Vec xlocal;
    PetscCall( DMCreateLocalVector(grid,&xlocal) ); 
    PetscCall( VecSet(xlocal,one) ); 
    PetscCall( DMLocalToGlobalBegin(grid,xlocal,INSERT_VALUES,x) ); 
    PetscCall( DMLocalToGlobalEnd(grid,xlocal,INSERT_VALUES,x) ); 
    PetscCall( VecDestroy(&xlocal) ); 
  }

  /*
   * Solve a linear system on the grid
   */
  KSP solver;
  PetscCall( KSPCreate(comm,&solver) ); 
  PetscCall( KSPSetType(solver,KSPBCGS) ); 
  /*
   * Exercise:
   * -- there is a line missing here.
   *    Run the code and try to deduce from the error message what you missed.
   *    (Or look anywhere else for inspiration.)
   */
/**** your code here ****/
  PetscCall( KSPSetFromOptions(solver) ); 
  PetscCall( KSPSolve(solver,x,y) ); 

  /*
   * Report on success of the solver, or lack thereof
   */
  {
    PetscInt its; KSPConvergedReason reason; 
    PetscCall( KSPGetConvergedReason(solver,&reason) );
    PetscCall( KSPGetIterationNumber(solver,&its) ); 
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
  PetscCall( KSPDestroy(&solver) ); 
  PetscCall( VecDestroy(&x) ); 
  PetscCall( VecDestroy(&y) ); 
  PetscCall( MatDestroy(&A) ); 
  PetscCall( DMDestroy(&grid) ); 

  PetscFinalize();
  return 0;
}
