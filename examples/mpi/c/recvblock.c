/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-7
   %%%%
   %%%% WRONG: guaranteed deadlock
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int other, recvbuf=2, sendbuf=3;

#include "globalinit.c"
  MPI_Status status;

//codesnippet recvblock
  other = 1-procno;
//codesnippet end
  if (procno>1) goto skip;
//codesnippet recvblock
  MPI_Recv(&recvbuf,1,MPI_INT,other,0,comm,&status);
  MPI_Send(&sendbuf,1,MPI_INT,other,0,comm);
  printf("This statement will not be reached on %d\n",procno);
//codesnippet end
  
 skip:
  MPI_Finalize();
  return 0;
}
