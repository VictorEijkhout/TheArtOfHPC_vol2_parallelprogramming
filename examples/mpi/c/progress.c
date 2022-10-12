/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% asynchronous progress examples
   %%%% https://software.intel.com/en-us/mpi-developer-guide-windows-async-progress-sample-c
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // for memset
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

#define PROGRESS_THREAD_COUNT 4
  if (nprocs<PROGRESS_THREAD_COUNT) {
  printf("Need at least %d procs\n",PROGRESS_THREAD_COUNT);
    MPI_Abort(comm,0);
  } else if (procno==0)
    printf("Running on %d proc\n",nprocs);

#define N 1000000
  size_t bufsize = PROGRESS_THREAD_COUNT * N;
  float large_buffer[bufsize];
  memset(large_buffer,1,bufsize*sizeof(float));

  MPI_Comm comms[PROGRESS_THREAD_COUNT];
  MPI_Request requests[PROGRESS_THREAD_COUNT];
  MPI_Info info;

  /* create “per-thread” communicators and assign thread id for each communicator */
  MPI_Comm *commdupallcomms;
  commdupallcomms = (MPI_Comm*) malloc( PROGRESS_THREAD_COUNT*sizeof(MPI_Comm) );
  for (int idx = 0; idx < PROGRESS_THREAD_COUNT; idx++) {
    MPI_Comm_dup(MPI_COMM_WORLD, &commdupallcomms[idx]);
    char thread_id_str[256] = { 0 };
    sprintf(thread_id_str, "%d", idx);
    MPI_Info_create(&info);
    MPI_Info_set(info, "thread_id", thread_id_str);
    MPI_Comm_set_info(comms[idx], info);
    MPI_Info_free(&info);
  }

  MPI_Barrier(comm);
  double tstart = MPI_Wtime();
  /* distribute MPI operations between communicators – i.e. between progress threads */
  for (int idx = 0; idx < PROGRESS_THREAD_COUNT; idx++)
    MPI_Iallreduce
      ( MPI_IN_PLACE, large_buffer + idx * PROGRESS_THREAD_COUNT,bufsize,MPI_FLOAT,
	MPI_MAX, comms[idx], &requests[idx]);

  MPI_Waitall(PROGRESS_THREAD_COUNT, requests, MPI_STATUSES_IGNORE );
  MPI_Barrier(comm);
  double duration = MPI_Wtime() - tstart;
  if (procno==0)
    printf("Running time: %e\n",duration);

  return 0;
}
