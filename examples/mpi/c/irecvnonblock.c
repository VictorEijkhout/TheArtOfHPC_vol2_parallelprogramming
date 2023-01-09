/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-5
   %%%%
   %%%% irecvnonblock.c : non-blocking exchange of data
   %%%%     between two processes
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

  int other, recvbuf, sendbuf;

//codesnippet irecvnonblock
  MPI_Request request[2];
  MPI_Status status[2];
//codesnippet end

  recvbuf = -1; sendbuf = procno+nprocs;
//codesnippet irecvnonblock
  other = nprocs-procno;
//codesnippet end
  if (procno==other) goto skip;
//codesnippet irecvnonblock
  MPI_Irecv(&recvbuf,1,MPI_INT,other,0,comm,&(request[0]));
  MPI_Isend(&sendbuf,1,MPI_INT,other,0,comm,&(request[1]));
  MPI_Waitall(2,request,status);
//codesnippet end
  
  if (recvbuf!=nprocs+other)
    printf("[%d] Receive mismatch: %d s/b %d\n",procno,recvbuf,nprocs+other);

 skip:
  MPI_Finalize();
  return 0;
}
