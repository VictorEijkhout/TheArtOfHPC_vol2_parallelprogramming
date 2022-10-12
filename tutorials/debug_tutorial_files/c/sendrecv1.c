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
  double myvalue=1.,rvalue;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);

  /*
   * Logically speaking, this should deadlock.
   * In practice, does it?
   * If not, replace MPI_Send by MPI_Ssend 
   * with the exact same parameter list.
   */
  for (int p=0; p<ntids; p++) {
    if (p!=mytid)
      MPI_Send(&myvalue,1,MPI_DOUBLE,
	       /* to */ p,/* tag */ 0,comm);
  }
  for (int p=0; p<ntids; p++) {
    if (p!=mytid)
      MPI_Recv(&rvalue,1,MPI_DOUBLE,
	       /* from */ p,/* tag */ 0,comm,MPI_STATUS_IGNORE);
  }

  MPI_Finalize();
  return 0;
}
