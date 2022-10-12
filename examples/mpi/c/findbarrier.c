/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2018
   %%%%
   %%%% findbarrier.c : illustrating MPI_Ibarrier
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc,char **argv) {
  MPI_Comm comm;
  int nprocs,procid;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procid);

  int mysleep;
  srand(procid*time(NULL));
  mysleep = nprocs * (rand()/(double)RAND_MAX);
  printf("[%d] working for %d seconds\n",procid,mysleep);
  sleep(mysleep);

  printf("[%d] finished, now posting barrier\n",procid);
  MPI_Request final_barrier;
  MPI_Ibarrier(comm,&final_barrier);

  int global_finish=mysleep;
  do {
    int all_done_flag=0;
    MPI_Test(&final_barrier,&all_done_flag,MPI_STATUS_IGNORE);
    if (all_done_flag) {
      break;
    } else {
      int flag; MPI_Status status;
      // force progress
      MPI_Iprobe
        ( MPI_ANY_SOURCE,MPI_ANY_TAG,
          comm, &flag, MPI_STATUS_IGNORE );
      printf("[%d] going to work for another second\n",procid);
      sleep(1);
      global_finish++;
    }
  } while (1);

  MPI_Wait(&final_barrier,MPI_STATUS_IGNORE);
  printf("[%d] concluded %d work, total time %d\n",
	 procid,mysleep,global_finish);

  MPI_Finalize();
  return 0;
}
