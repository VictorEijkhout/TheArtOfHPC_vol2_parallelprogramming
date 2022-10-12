/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% thread.c : init threaded MPI
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "omp.h"

int main(int argc,char **argv) {

  int procid,nprocs;
  int requested=MPI_THREAD_MULTIPLE,provided;

  MPI_Init_thread(&argc,&argv,requested,&provided);
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procid);
  MPI_Comm_size(comm,&nprocs);
  if (procid==0)
    printf("Threading level requested=%d, provided=%d\n",
	   requested,provided);

  int ncores;
#pragma omp parallel
#pragma omp master
  ncores = omp_get_num_procs();

  int totalcores;
  MPI_Reduce(&ncores,&totalcores,1,MPI_INT,MPI_SUM,0,comm);
  if (procid==0) {
    printf("Omp procs on this process: %d\n",ncores);
    printf("Omp procs total: %d\n",totalcores);
  }

  MPI_Finalize();
  return 0;
}
