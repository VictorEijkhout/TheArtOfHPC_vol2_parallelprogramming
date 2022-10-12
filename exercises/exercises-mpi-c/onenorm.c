/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2019
   %%%%
   %%%% onenorm.c : user-defined one-norm
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

void print_final_result( int cond,MPI_Comm comm );

void one_norm(void *in,void *inout,int *len,MPI_Datatype *type) {
  // r is the already reduced value, n is the new value
  float n = *(float*)in, r = *(float*)inout;
  float m;

  /*
   * Exercise:
   * -- write the correct reduction step 
   *    for computing a 1-norm
   */
/**** your code here ****/
#ifdef DEBUG
  printf("combine %e %e : %d\n",r,n,m);
#endif
  *(float*)inout = m;  
}

int main(int argc,char **argv) {

  MPI_Comm comm;
  MPI_Init(&argc,&argv); 
  comm = MPI_COMM_WORLD;

  int procno=-1,nprocs,err;
  MPI_Comm_rank(comm,&procno); 
  MPI_Comm_size(comm,&nprocs); 

  /* 
   * Data specific for this program:
   * every process stores one number, minus-one,
   * which makes the one-norm equal to the 
   * number of processes.
   */  
  float data,data_one_norm;
  data = -1.e0;

  /*
   * Create the operator,
   * do the reduction,
   * free the operator.
   */
  MPI_Op calc_one;
  MPI_Op_create(one_norm,1,&calc_one);
  MPI_Allreduce(&data,&data_one_norm,1,MPI_FLOAT,calc_one,comm);
  MPI_Op_free(&calc_one);

  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  /*
   * Correctness test:
   *  check that the distributed result is the same as sequential
   */
  float actual_norm = nprocs;
  int error_test = data_one_norm!=actual_norm;
  print_final_result(error_test,comm);
  
  /* if () { */
  /*   printf("[%d] Result %e should be %e\n", */
  /*          procno,data_one_norm,actual_norm); */
  /* } else if (procno==0) { */
  /*   printf("User-defined reduction successful: %e\n",data_one_norm); */
  /* } */

  MPI_Finalize();
  return 0;
}
