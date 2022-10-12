/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% bcast.c : code with a parallelism bug
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "mpi.h"

/*
 * This program has an incorrect use of the MPI_Bcast call
 * It may hang, but probably only for the parameter N large enough.
 * Do:
 *     mpicc bcast.c -DN=50000
 * (for large enough value)
 * to see this behavior.
 */
#ifndef N
#define N 100
#endif

int main(int argc,char **argv) {
  MPI_Comm comm;
  int mytid,ntids, p;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);

  double buffer[N];
  memset(buffer,0,N*sizeof(double));
  if (mytid==0) 
    printf("Broadcast of %d doubles\n",N);

  MPI_Bcast( buffer,N,MPI_DOUBLE,mytid,comm);
  MPI_Barrier(comm);

  if (mytid==0) 
    printf("Finished: %e\n",buffer[0]);

  MPI_Finalize();
  return 0;
}
