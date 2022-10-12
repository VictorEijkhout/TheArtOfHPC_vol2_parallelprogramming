/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% reducescatter.c : simple illustration of reduce_scatter
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  /* 
   * Set up an array of which processes you will receive from
   */
  int
    // data that we know:
    *i_recv_from_proc = (int*) malloc(nprocs*sizeof(int)),
    *procs_to_recv_from, nprocs_to_recv_from=0,
    // data we are going to determin:
    *procs_to_send_to,nprocs_to_send_to;

  /*
   * Initialize
   */
  for (int i=0; i<nprocs; i++) {
    i_recv_from_proc[i] = 0;
  }

  /*
   * Generate array of "yes/no I recv from proc p",
   * and condensed array of procs I receive from.
   */
  nprocs_to_recv_from = 0;
  for (int iproc=0; iproc<nprocs; iproc++)
    // pick random procs to receive from, not yourself.
    if ( (float) rand()/(float)RAND_MAX < 2./nprocs && iproc!=procno ) {
      i_recv_from_proc[iproc] = 1;
      nprocs_to_recv_from++;
    }
  procs_to_recv_from = (int*) malloc(nprocs_to_recv_from*sizeof(int));
  int count_procs_to_recv_from = 0;
  for (int iproc=0; iproc<nprocs; iproc++)
    if ( i_recv_from_proc[iproc] )
      procs_to_recv_from[count_procs_to_recv_from++] = iproc;
  ASSERT( count_procs_to_recv_from==nprocs_to_recv_from );

  /*
   */
  printf("[%d] receiving from:",procno);
  for (int iproc=0; iproc<nprocs_to_recv_from; iproc++)
    printf(" %3d",procs_to_recv_from[iproc]);
  printf(".\n");

  /*
   * Now find how many procs will send to you
   */
  MPI_Reduce_scatter_block
    (i_recv_from_proc,&nprocs_to_send_to,1,MPI_INT,
    MPI_SUM,comm);

  /*
   * Send a zero-size msg to everyone that you receive from,
   * just to let them know that they need to send to you.
   */
  MPI_Request send_requests[nprocs_to_recv_from];
  for (int iproc=0; iproc<nprocs_to_recv_from; iproc++) {
    int proc=procs_to_recv_from[iproc];
    double send_buffer=0.;
    MPI_Isend(&send_buffer,0,MPI_DOUBLE, /*to:*/ proc,0,comm,
	      &(send_requests[iproc]));
  }

  /*
   * Do as many receives as you know are coming in;
   * use wildcards since you don't know where they are coming from.
   * The source is a process you need to send to.
   */
  procs_to_send_to = (int*)malloc( nprocs_to_send_to * sizeof(int) );
  for (int iproc=0; iproc<nprocs_to_send_to; iproc++) {
    double recv_buffer;
    MPI_Status status;
    MPI_Recv(&recv_buffer,0,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,comm,
	     &status);
    procs_to_send_to[iproc] = status.MPI_SOURCE;
  }
  MPI_Waitall(nprocs_to_recv_from,send_requests,MPI_STATUSES_IGNORE);

  printf("[%d] sending to:",procno);
  for (int iproc=0; iproc<nprocs_to_send_to; iproc++)
    printf(" %3d",procs_to_send_to[iproc]);
  printf(".\n");

  MPI_Finalize();
  return 0;
}
