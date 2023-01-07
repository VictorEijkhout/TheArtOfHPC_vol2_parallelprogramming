/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% irecvloop.c : use non-blocking send/recv to communicate
   %%%%     all-to-all with left/right neighbours in a circular fashion
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

  int recvbuf=2, sendbuf=3, other;
  //codesnippet irecvloop
  MPI_Request requests = 
    (MPI_Request*) malloc( 2*nprocs*sizeof(MPI_Request) );
  recv_buffers = (int*) malloc( nprocs*sizeof(int) );
  send_buffers = (int*) malloc( nprocs*sizeof(int) );
  for (int p=0; p<nprocs; p++) {
    int
      left_p  = (p-1+nprocs) % nprocs,
      right_p = (p+1) % nprocs;
    send_buffer[p] = nprocs-p;
    MPI_Isend(sendbuffer+p,1,MPI_INT, right_p,0, requests+2*p);
    MPI_Irecv(recvbuffer+p,1,MPI_INT, left_p,0, requests+2*p+1);
  }
  /* your useful code here */
  MPI_Waitall(2*nprocs,requests,MPI_STATUSES_IGNORE);
  //codesnippet end
  
 skip:
  MPI_Finalize();
  return 0;
}
