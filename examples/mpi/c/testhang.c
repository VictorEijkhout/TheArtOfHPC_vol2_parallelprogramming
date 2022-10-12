/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% testhang.c : show that MPI_Test does not force completion
   %%%% DOES NOT WORK (in the sense that it works....)
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  // Initialize the random number generator
  srand((int)(procno*(double)RAND_MAX/nprocs));

  int sender = 0, receiver = nprocs-1;
#define N 10000000
  int count = N;
  float buffer[N];
#define SLEEP 1
  int nsleep = SLEEP;

  if (procno==receiver) {
    // post a receive
    MPI_Request recv_request;
    MPI_Irecv(buffer,count,MPI_FLOAT, MPI_ANY_SOURCE,MPI_ANY_TAG,comm,&recv_request);
    // test completion
    int recv_completed = 0;
    while (!recv_completed) {
      printf("sleep %d\n",nsleep); sleep(nsleep);
      MPI_Test(&recv_request,&recv_completed,MPI_STATUS_IGNORE);
    }
    printf("recv completed\n");
  } else if (procno==sender) {
    ierr = MPI_Send(buffer,count,MPI_FLOAT, receiver,0,comm); CHK(ierr);
  }

  MPI_Finalize();
  return 0;
}
