/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% sendblock.c : illustration of the eager limit
   %%%%
   %%%% WRONG: unpredictable deadlock
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

/** This program shows deadlocking behaviour
    when two processes exchange data with blocking
    send and receive calls.
    Under a certain limit MPI_Send may actually not be
    blocking; we loop, increasing the message size,
    to find roughly the crossover point.
*/
int main(int argc,char **argv) {
  int  *recvbuf, *sendbuf;
  MPI_Status status;

#include "globalinit.c"

  /* we only use processors 0 and 1 */
  int other;
  if (procno>1) goto skip;
  other = 1-procno;
  /* loop over increasingly large messages */
  for (int size=1; size<2000000000; size*=10) {
    sendbuf = (int*) malloc(size*sizeof(int));
    recvbuf = (int*) malloc(size*sizeof(int));
    if (!sendbuf || !recvbuf) {
      printf("Out of memory\n"); MPI_Abort(comm,1);
    }
    MPI_Send(sendbuf,size,MPI_INT,other,0,comm);
    MPI_Recv(recvbuf,size,MPI_INT,other,0,comm,&status);
    /* If control reaches this point, the send call
       did not block. If the send call blocks,
       we do not reach this point, and the program will hang.
    */
    if (procno==0)
      printf("Send did not block for size %d\n",size);
    free(sendbuf); free(recvbuf);
  }
  
 skip:
  MPI_Finalize();
  return 0;
}
