/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% ssendblock.c : with synchronous sends not eager limit applies
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
  int other, size, *recvbuf, *sendbuf;
  MPI_Status status;

#include "globalinit.c"

  if (procno>1) goto skip;
  other = 1-procno;
  sendbuf = (int*) malloc(sizeof(int));
  recvbuf = (int*) malloc(sizeof(int));
  size = 1;
  MPI_Ssend(sendbuf,size,MPI_INT,other,0,comm);
  MPI_Recv(recvbuf,size,MPI_INT,other,0,comm,&status);
  printf("This statement is not reached\n");
  free(sendbuf); free(recvbuf);
  
 skip:
  MPI_Finalize();
  return 0;
}
