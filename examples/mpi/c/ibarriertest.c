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

#include "globalinit.c"

  /*
   * Pick one random process 
   * that will do a send
   */
  int sender,receiver;
  if (procno==0)
    sender = rand()%nprocs;
  MPI_Bcast(&sender,1,MPI_INT,0,comm);
  int i_do_send = sender==procno;

  float data=1.;
  MPI_Request send_request;

  if (i_do_send) {
    /*
     * Pick a random process to send to, 
     * not yourself.
     */
    int receiver = rand()%nprocs;

    while (receiver==procno) receiver = rand()%nprocs;
    printf("[%d] random send performed to %d\n",procno,receiver);
    //MPI_Isend(&data,1,MPI_FLOAT,receiver,0,comm,&send_request);

    //    MPI_Ssend(&data,1,MPI_FLOAT,receiver,0,comm);
  }
  /*
   * Everyone posts the non-block barrier
   * and gets a request to test/wait for
   */
  MPI_Request barrier_request;
  MPI_Ibarrier(comm,&barrier_request);


  int step=0;
  /*
   * Now everyone repeatedly tests the barrier
   * and probes for incoming message.
   * If the barrier completes, there are no
   * incoming message.
   */
  MPI_Barrier(comm);
  double tstart = MPI_Wtime();
  for ( ; ; step++) {
    int barrier_done_flag=0;
    MPI_Test(&barrier_request,&barrier_done_flag,MPI_STATUS_IGNORE);
    if (barrier_done_flag) {
      break;
    } else {
      // if we do not probe, there will be no progress
      /* int flag; MPI_Status status; */
      /* MPI_Iprobe */
      /*   ( MPI_ANY_SOURCE,MPI_ANY_TAG, */
      /*     comm, &flag, &status ); */
    }
  }

  printf("[%d] concluded after %d steps\n",procno,step);
  MPI_Wait(&barrier_request,MPI_STATUS_IGNORE);

  MPI_Barrier(comm);
  double duration = MPI_Wtime()-tstart;
  if (procno==0) printf("Test loop: %e\n",duration);

  //  MPI_Recv(&data,1,MPI_FLOAT,sender,0,comm,MPI_STATUS_IGNORE);
  printf("[%d] random receive from %d\n",procno,sender);

  /* if (i_do_send) */
  /*   MPI_Wait(&send_request,MPI_STATUS_IGNORE); */

  MPI_Finalize();
  return 0;
}
