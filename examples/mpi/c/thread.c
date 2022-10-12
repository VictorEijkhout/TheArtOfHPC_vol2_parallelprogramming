/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-5
   %%%%
   %%%% thread.c : initializing MPI for multi-threaded
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm;
  int procno=-1,nprocs,threading,err;
  
  MPI_Init_thread(&argc,&argv,MPI_THREAD_MULTIPLE,&threading);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  if (procno==0) {
    switch (threading) {
    case MPI_THREAD_MULTIPLE : printf("Glorious multithreaded MPI\n"); break;
    case MPI_THREAD_SERIALIZED : printf("No simultaneous MPI from threads\n"); break;
    case MPI_THREAD_FUNNELED : printf("MPI from main thread\n"); break;
    case MPI_THREAD_SINGLE : printf("no threading supported\n"); break;
    }
  }
  MPI_Finalize();
  return 0;
}
