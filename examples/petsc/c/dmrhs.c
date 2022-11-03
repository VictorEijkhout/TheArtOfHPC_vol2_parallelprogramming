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

  /*
   * Create a 2d grid and a matrix on that grid.
   */
  DM grid;
  ierr = DMDACreate2d
    ( comm,          
      DM_BOUNDARY_NONE,DM_BOUNDARY_NONE,
      DMDA_STENCIL_STAR,
      100,100,          
      PETSC_DECIDE,PETSC_DECIDE,
      1,         
      1,         
      NULL,NULL, 
      &grid      
      ); CHKERRQ(ierr);
  ierr = DMSetFromOptions(grid); CHKERRQ(ierr);
  ierr = DMSetUp(grid); CHKERRQ(ierr);
  ierr = DMViewFromOptions(grid,NULL,"-dm_view"); CHKERRQ(ierr);

  /*
   * Print out how the grid is distributed over processors
   */
  DMDALocalInfo  info;
  ierr = DMDAGetLocalInfo(grid,&info); CHKERRQ(ierr);
  ierr = PetscPrintf(comm,"Create\n"); CHKERRQ(ierr);
  ierr = PetscSynchronizedPrintf
    (comm,
     "[%d] Local = %d-%d x %d-%d, halo = %d-%d x %d-%d\n",
     procno,
     info.xs,info.xs+info.xm,info.ys,info.ys+info.ym,
     info.gxs,info.gxs+info.gxm,info.gys,info.gys+info.gym
     ); CHKERRQ(ierr);
  ierr = PetscSynchronizedFlush(comm,stdout); CHKERRQ(ierr);
  ierr = PetscPrintf(comm,"create\n"); CHKERRQ(ierr);

  Vec xy;
  ierr = VecCreate(comm,&xy); CHKERRQ(ierr);
  ierr = VecSetType(xy,VECMPI); CHKERRQ(ierr);
  PetscInt nlocal = info.xm*info.ym, nglobal = info.mx*info.my;
  ierr = VecSetSizes(xy,nlocal,nglobal); CHKERRQ(ierr);
  {
    PetscReal
      hx = 1. / ( info.mx-1 ),
      hy = 1. / ( info.my-1 );
    PetscReal **xyarray;
    DMDAVecGetArray(grid,xy,&xyarray); CHKERRQ(ierr);
    for (int j=info.ys; j<info.ys+info.ym; j++) {
      for (int i=info.xs; i<info.xs+info.xm; i++) {
        PetscReal x = i*hx, y = j*hy;
        xyarray[j][i] = x*y;
      }
    }
    DMDAVecRestoreArray(grid,xy,&xyarray); CHKERRQ(ierr);
  }
  ierr = VecAssemblyBegin(xy); CHKERRQ(ierr);
  ierr = VecAssemblyEnd(xy); CHKERRQ(ierr);
  //  ierr = VecView(xy,0); CHKERRQ(ierr);
  
  {
    Vec ghostvector;
    ierr = DMGetLocalVector(grid,&ghostvector); CHKERRQ(ierr);
    ierr = DMGlobalToLocal(grid,xy,INSERT_VALUES,ghostvector); CHKERRQ(ierr);
    PetscReal **xyarray,**gh;
    ierr = DMDAVecGetArray(grid,xy,&xyarray); CHKERRQ(ierr);
    ierr = DMDAVecGetArray(grid,ghostvector,&gh); CHKERRQ(ierr);
    // computation on the arrays
    for (int j=info.ys; j<info.ys+info.ym; j++) {
      for (int i=info.xs; i<info.xs+info.xm; i++) {
	if (info.gxs<info.xs && info.gys<info.ys)
	  if (i-1>=info.gxs && i+1<=info.gxs+info.gxm &&
	      j-1>=info.gys && j+1<=info.gys+info.gym )
	    xyarray[j][i] =
	      ( gh[j-1][i] + gh[j][i-1] + gh[j][i+1] + gh[j+1][i] )
	      /4.;
	goto exit;
      }
    } exit:    
    ierr = DMDAVecRestoreArray(grid,xy,&xyarray); CHKERRQ(ierr);
    ierr = DMDAVecRestoreArray(grid,ghostvector,&gh); CHKERRQ(ierr);
    ierr = DMLocalToGlobal(grid,ghostvector,INSERT_VALUES,xy); CHKERRQ(ierr);
    ierr = DMRestoreLocalVector(grid,&ghostvector); CHKERRQ(ierr);

    /* printf("x: %d+%d in %d+%d, y: %d+%d in %d+%d\n", */
    /* 	   info.xs,info.xm,info.gxs,info.gxm, */
    /* 	   info.ys,info.ym,info.gys,info.gym */
    /* 	   );  */
    //    if ( ! (info.ys==info.gys || info.ys==info.gys ) ) goto exit;
  }

  ierr = VecDestroy(&xy); CHKERRQ(ierr);
  ierr = DMDestroy(&grid); CHKERRQ(ierr);

  return PetscFinalize();
}
