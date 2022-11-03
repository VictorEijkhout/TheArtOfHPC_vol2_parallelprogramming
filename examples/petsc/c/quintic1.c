/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 ****************************************************************/

#include "petsc.h"
#define use_jacobian 1

PetscErrorCode FormFunction( SNES solver,Vec x,Vec fx,void *ctx) {
  PetscScalar *fv;
  const PetscScalar *xv;
  PetscErrorCode ierr;
  PetscFunctionBegin;
  ierr = VecGetArrayRead(x,&xv); CHKERRQ(ierr);
  ierr = VecGetArray(fx,&fv); CHKERRQ(ierr);
  {
    PetscScalar x = xv[0], y;
    y = x*x*x*x*x - x -3;
    printf("f( x=%e ) = %e\n",x,y);
    fv[0] = y;
  }
  ierr = VecRestoreArrayRead(x,&xv); CHKERRQ(ierr);
  ierr = VecRestoreArray(fx,&fv); CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

PetscErrorCode Jacobian( SNES solver,Vec x,Mat J,Mat P,void *ctx) {
  PetscScalar *jv;
  const PetscScalar *xv;
  PetscErrorCode ierr;
  PetscFunctionBegin;
  ierr = VecGetArrayRead(x,&xv); CHKERRQ(ierr);
  ierr = MatDenseGetArray(J,&jv); CHKERRQ(ierr);
  {
    PetscScalar x = xv[0], y;
    y = 4*x*x*x*x - 1;
    printf("j( x=%e ) = %e\n",x,y);
    jv[0] = y;
  }
  ierr = VecRestoreArrayRead(x,&xv); CHKERRQ(ierr);
  ierr = MatDenseRestoreArray(J,&jv); CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

int main(int argc,char **args)
{
  PetscErrorCode ierr;
  MPI_Comm comm;
  SNES solver;
  Vec x,fx;
  
  PetscFunctionBegin;
  PetscInitialize(&argc,&args,0,0);

  comm = PETSC_COMM_SELF;

  ierr = VecCreate(comm,&x); CHKERRQ(ierr);
  ierr = VecSetType(x,VECSEQ); CHKERRQ(ierr);
  ierr = VecSetSizes(x,1,1); CHKERRQ(ierr);
  ierr = VecSet(x,1.5); CHKERRQ(ierr);
  ierr = VecDuplicate(x,&fx); CHKERRQ(ierr);

  ierr = SNESCreate(comm,&solver); CHKERRQ(ierr);
  ierr = SNESSetFunction(solver,fx,FormFunction,NULL); CHKERRQ(ierr);
  Mat J;
  if (use_jacobian) {
    ierr = MatCreate(comm,&J); CHKERRQ(ierr);
    ierr = MatSetType(J,MATSEQDENSE); CHKERRQ(ierr);
    ierr = MatSetSizes(J,1,1,1,1); CHKERRQ(ierr);
    ierr = MatSetUp(J); CHKERRQ(ierr);
    ierr = SNESSetJacobian(solver,J,J,&Jacobian,NULL); CHKERRQ(ierr);
  }
  ierr = SNESSetFromOptions(solver); CHKERRQ(ierr);
  ierr = SNESSolve(solver,NULL,x); CHKERRQ(ierr);
  VecView(x,0);
  VecView(fx,0);

  ierr = VecDestroy(&x); CHKERRQ(ierr);
  ierr = VecDestroy(&fx); CHKERRQ(ierr);
  //  ierr = (); CHKERRQ(ierr);
  
  return PetscFinalize();
}

