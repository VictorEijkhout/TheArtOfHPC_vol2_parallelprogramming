/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2018
   %%%%
   %%%% ibarrierupdate.c : illustrating MPI_Ibarrier
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
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  // first set a unique random seed
  srand((int)(procno*(double)RAND_MAX/nprocs));

  /*
   * How many processes are we going to send to?
   */
  int n_destinations;
  n_destinations = nprocs * (float)rand() / (double)RAND_MAX;
  /*
   * Pick random processes to send to, and post an Isend
   */
  double send_data[n_destinations];
  MPI_Request send_requests[n_destinations];
  for (int idestination=0; idestination<n_destinations; idestination++) {
    int destination;
    destination = nprocs * (float)rand() / (double)RAND_MAX;
    printf("[%d] send to %d\n",procno,destination);
    MPI_Isend
      (send_data+idestination,1,MPI_DOUBLE,
       destination,0,comm, send_requests+idestination);
  }
  
  /*
   * When you're done sending, post a non-blocking barrier
   */
  printf("[%d] posting barrier\n",procno);
  MPI_Request final_barrier;
  MPI_Ibarrier(comm,&final_barrier);

  int step=0;
  for ( ; ; step++) {
    /*
     * Exercise part 1:
     * -- use MPI_Test to determine when the barrier is completed;
     * -- in that case you can quit
     */
    int all_done_flag=0;
    MPI_Test(&final_barrier,&all_done_flag,MPI_STATUS_IGNORE);
/**** your code here ****/

    /*
     * Exercise
     * -- use MPI_Iprobe to test if there is a message
     * part 1: what do you do if there is no message?
     */
    MPI_Status recv_status;
    double receive_data; 
    int message_flag;
    MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,comm,&message_flag,&recv_status);
/**** your code here ****/
    /*
     * part 2: the message can come from anywhere, so 
     *    you need to inspect the status to find the source and tag
     */
    int source;
/**** your code here ****/
    MPI_Recv
      (&receive_data,1,MPI_DOUBLE,
       source,MPI_ANY_TAG,comm,MPI_STATUS_IGNORE);
    printf("[%d] received from %d\n",procno,source);
  }

  printf("[%d] concluded after %d steps\n",procno,step);
  MPI_Wait(&final_barrier,MPI_STATUS_IGNORE);

  MPI_Finalize();
  return 0;
}
