/****************************************************************
 ****
 **** This program file is part of the tutorial
 **** `Introduction to the PETSc library'
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright Victor Eijkhout 2012-2024
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
    PetscCall( PetscOptionsGetInt(PETSC_NULL,PETSC_NULL,"-x",&x,NULL) ); 
    Nglobal *= x;
  }

  Vec in,out;
  PetscCall( VecCreate(comm,&in) ); 
  PetscCall( VecSetType(in,VECMPI) ); 
  PetscCall( VecSetSizes(in,PETSC_DECIDE,Nglobal) ); 
  PetscCall( VecDuplicate(in,&out) ); 
  
  {
    PetscInt myfirst,mylast;
    PetscCall( VecGetOwnershipRange(in,&myfirst,&mylast) ); 
    for (PetscInt index=myfirst; index<mylast; index++) {
      PetscScalar v = index;
      PetscCall( VecSetValue(in,index,v,INSERT_VALUES) ); 
    }
    PetscCall( VecAssemblyBegin(in) ); 
    PetscCall( VecAssemblyEnd(in) ); 
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
    //    PetscCall( ISCreateStride(comm,Nglobal/2,0,2,&oddeven) ); 
/**** your code here ****/
  } else {
    //    PetscCall( ISCreateStride(comm,Nglobal/2,1,2,&oddeven) ); 
/**** your code here ****/
  }
  ISView(oddeven,0);

  VecScatter separate;
  PetscCall( VecScatterCreate(in,oddeven,out,NULL,&separate) ); 
  PetscCall( VecScatterBegin(separate,in,out,INSERT_VALUES,SCATTER_FORWARD) ); 
  PetscCall( VecScatterEnd(separate,in,out,INSERT_VALUES,SCATTER_FORWARD) ); 

  PetscCall( ISDestroy(&oddeven) ); 
  PetscCall( VecScatterDestroy(&separate) ); 

  PetscCall( VecView(in,0) ); 
  PetscCall( VecView(out,0) ); 

  PetscCall( VecDestroy(&in) ); 
  PetscCall( VecDestroy(&out) ); 

  PetscFunctionReturn(0);
}

