/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% Debugging example
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc,char **argv) {
  MPI_Comm comm;
  int mytid,ntids;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);

  double sum=0;
  /*
   * Attempted solution for the deadlock:
   * Does this still deadlock?
   * Note: we force deadlock by using MPI_Ssend
   */
  // stage 1: we send to the right
  for (int p=0; p<mytid; p++) {
    double rvalue;
    MPI_Recv(&rvalue,1,MPI_DOUBLE,
	     /* from */ p,/* tag */ 0,comm,MPI_STATUS_IGNORE);
    sum += rvalue;
  }
  for (int p=mytid+1; p<ntids; p++) {
    double myvalue = mytid+1;
    MPI_Ssend(&myvalue,1,MPI_DOUBLE,
	      /* to */ p,/* tag */ 0,comm);
  }
  // stage 2: we send to the left
  for (int p=ntids-1; p>mytid; p--) {
    double rvalue;
    MPI_Recv(&rvalue,1,MPI_DOUBLE,
	     /* from */ p,/* tag */ 0,comm,MPI_STATUS_IGNORE);
    sum += rvalue;
  }
  for (int p=mytid-1; p>=0; p--) {
    double myvalue = mytid+1;
    MPI_Ssend(&myvalue,1,MPI_DOUBLE,
	      /* to */ p,/* tag */ 0,comm);
  }

  printf("[%d] Compare: received=%e, s/b=%e\n",
	 mytid,sum,ntids*(ntids+1)/2.-mytid-1);

  MPI_Finalize();
  return 0;
}
