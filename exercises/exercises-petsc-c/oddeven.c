/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** 
 **** oddeven.c : split a parallel vector into odd/even indices
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
  MPI_Comm comm;
  Vec Rhs,Sol;
  
  PetscFunctionBegin;
  PetscInitialize(&argc,&args,0,0);

  comm = MPI_COMM_WORLD;

  int nprocs,procid;
  MPI_Comm_rank(comm,&procid);
  MPI_Comm_size(comm,&nprocs);
  if (nprocs!=2) {
    PetscPrintf(comm,"This example only works on 2 processes, not %d\n",nprocs);
    PetscFunctionReturn(-1); }

  PetscInt Nglobal = 2*nprocs;
  /*
   * Multiplication factor:
   * run your code as
   *     oddeven -x 3
   * to multiply the length of the vector by 3
   */
  {
    PetscInt x=4;
    ierr = PetscOptionsGetInt(PETSC_NULL,PETSC_NULL,"-x",&x,NULL); CHKERRQ(ierr);
    Nglobal *= x;
  }

  Vec in,out;
  ierr = VecCreate(comm,&in); CHKERRQ(ierr);
  ierr = VecSetType(in,VECMPI); CHKERRQ(ierr);
  ierr = VecSetSizes(in,PETSC_DECIDE,Nglobal); CHKERRQ(ierr);
  ierr = VecDuplicate(in,&out); CHKERRQ(ierr);
  
  {
    PetscInt myfirst,mylast;
    ierr = VecGetOwnershipRange(in,&myfirst,&mylast); CHKERRQ(ierr);
    for (PetscInt index=myfirst; index<mylast; index++) {
      PetscScalar v = index;
      ierr = VecSetValue(in,index,v,INSERT_VALUES); CHKERRQ(ierr);
    }
    ierr = VecAssemblyBegin(in); CHKERRQ(ierr);
    ierr = VecAssemblyEnd(in); CHKERRQ(ierr);
  }

  /*
   * Exercise:
   * -- code is given here for splitting a vector into odd/even indices
   * -- change the IS objects so that indices will additionally be
   *    ordered from high to low index
   */
  IS oddeven;
  if (procid==0) {
    /*
     * here is code for the original ordering, first enable this,
     * then figure out how to reverse it.
     */
    //    ierr = ISCreateStride(comm,Nglobal/2,0,2,&oddeven); CHKERRQ(ierr);
/**** your code here ****/
  } else {
    //    ierr = ISCreateStride(comm,Nglobal/2,1,2,&oddeven); CHKERRQ(ierr);
/**** your code here ****/
  }
  ISView(oddeven,0);

  VecScatter separate;
  ierr = VecScatterCreate
    (in,oddeven,out,NULL,&separate); CHKERRQ(ierr);
  ierr = VecScatterBegin
    (separate,in,out,INSERT_VALUES,SCATTER_FORWARD); CHKERRQ(ierr);
  ierr = VecScatterEnd
    (separate,in,out,INSERT_VALUES,SCATTER_FORWARD); CHKERRQ(ierr);

  ierr = ISDestroy(&oddeven); CHKERRQ(ierr);
  ierr = VecScatterDestroy(&separate); CHKERRQ(ierr);

  ierr = VecView(in,0); CHKERRQ(ierr);
  ierr = VecView(out,0); CHKERRQ(ierr);

  ierr = VecDestroy(&in); CHKERRQ(ierr);
  ierr = VecDestroy(&out); CHKERRQ(ierr);

  PetscFunctionReturn(0);
}

