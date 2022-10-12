/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% count.c : count what's in the receive buffer
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv) {
  MPI_Comm comm;
  int nprocs,procid;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procid);

#define N 10
  float buffer[N];
  int target = nprocs-1;
  if (procid==0) {
    int sendcount = (rand()>.5) ? N : N-1;
    MPI_Send( buffer,sendcount,MPI_FLOAT,target,0,comm );
  } else if (procid==target) {
    MPI_Status status;
    int recvcount;
    MPI_Recv( buffer,N,MPI_FLOAT,0,0, comm, &status );
    MPI_Get_count(&status,MPI_FLOAT,&recvcount);
    printf("Received %d elements\n",recvcount);
  }

  MPI_Finalize();
  return 0;
}
