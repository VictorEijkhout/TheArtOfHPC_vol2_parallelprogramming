/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** MPI Exercise to illustrate pipelining
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

double array_error(double a[],double b[],int array_size);
void print_final_result( int cond,MPI_Comm comm );

#ifndef N
#define N 10
#endif

int main(int argc,char **argv) {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Init(&argc,&argv);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  // Set `sendto' and `recvfrom'
  int sendto =
    ( procno<nprocs-1 ? procno+1 : MPI_PROC_NULL )
    ;
  int recvfrom =
    ( procno>0 ? procno-1 : MPI_PROC_NULL )
    ;

  //
  // Exercise:
  // -- do the MPI_Send and MPI_Recv calls
  //    so that you add your square value to the data from the previous
  //    and pass it on to the next.
  //
  double leftdata[N], myvalue[N];
  for (int i=0; i<N; i++) leftdata[i] = 0.;
/**** your code here ****/


  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  double answers[N];
  for (int i=0; i<N; i++) {
    double p1 = procno+1.;
    answers[i] = p1*p1*p1/3 + p1*p1/2 + p1/6;
  }
  double relative_error = array_error(answers,myvalue,N);
  //  printf("[%d] relative error=%e\n",procno,relative_error);
  print_final_result( relative_error > 1.e-12, comm );

  MPI_Finalize();
  return 0;
}

