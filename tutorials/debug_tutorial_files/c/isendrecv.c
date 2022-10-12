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
  MPI_Request *requests;
  int mytid,ntids, p;
  double myvalue=1.;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);
  requests = (MPI_Request*) malloc(ntids*sizeof(MPI_Request));

  /*
   * Using Isend definitely doesn't deadlock
   */
  for (p=0; p<ntids; p++) {
    MPI_Isend(&myvalue,1,MPI_DOUBLE,
	      /* to */ p,/* tag */ 0,comm,requests+p);
  }
  for (p=0; p<ntids; p++) {
    MPI_Recv(&myvalue,1,MPI_DOUBLE,
	     /* from */ p,/* tag */ 0,comm,MPI_STATUS_IGNORE);
  }
  MPI_Waitall(ntids,requests,MPI_STATUSES_IGNORE);

  MPI_Finalize();
  return 0;
}
