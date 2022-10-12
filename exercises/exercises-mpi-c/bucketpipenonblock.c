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

#include "tools.h"

#ifndef N
#define N 10
#endif

#ifndef PARTS
#define PARTS 2
#endif

int main(int argc,char **argv) {

  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;

  int nprocs, procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);


  // Set `sendto' and `recvfrom'
  int sendto =
    ( procno<nprocs-1 ? procno+1 : MPI_PROC_NULL )
    ;
  int recvfrom =
    ( procno>0 ? procno-1 : MPI_PROC_NULL )
    ;

  // Set up data array
  double leftdata[N], myvalue[N];
  for (int i=0; i<N; i++) leftdata[i] = 0.;

  // Set up blocking for the pipeline
  int partition_starts[PARTS], partition_sizes[PARTS];
  { int points_left=N, block = N/PARTS;
    for (int ipart=0; ipart<PARTS; ipart++) {
      partition_starts[ipart] = ipart*block;
      if (ipart<PARTS-1)
	partition_sizes[ipart] = block;
      else
	partition_sizes[ipart] = points_left;
      points_left -= partition_sizes[ipart];
      if (points_left<0) {
	printf("Can not partition N=%d over PARTS=%d\n",N,PARTS);
	MPI_Abort(comm,1);
      }
    }
  }

  //
  // Exercise:
  // The code here is using blocking sends and receives.
  // Replace by non-blocking.
  //
#if 0
  for (int ipart=0; ipart<PARTS; ipart++) {
    MPI_Recv
      (
       leftdata+partition_starts[ipart],partition_sizes[ipart],
       MPI_DOUBLE,recvfrom,ipart,comm,MPI_STATUS_IGNORE);
    for (int i=partition_starts[ipart];
	 i<partition_starts[ipart]+partition_sizes[ipart];
	 i++)
      myvalue[i] = (procno+1)*(procno+1) + leftdata[i];
    MPI_Send
      (
       myvalue+partition_starts[ipart],partition_sizes[ipart],
       MPI_DOUBLE,sendto,ipart,comm);
  }
#endif
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
  int error_test = relative_error > 1.e-12;
  if (error_test)
    printf("[%d] relative error=%e\n",procno,relative_error);
  print_final_result( error_test, comm );

  MPI_Finalize();
  return 0;
}
