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
#include <math.h>
#include "mpi.h"

int main(int argc,char **argv) {
  MPI_Comm comm;
  MPI_Request *requests;
  int mytid,ntids, p;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);
  requests = (MPI_Request*) malloc(ntids*sizeof(MPI_Request));

  /*
   * Create receive buffers
   */
  double *rvalues;
  rvalues = (double*)malloc(ntids*sizeof(double));

  /*
   * Using Isend definitely doesn't deadlock
   */
  for (p=0; p<ntids; p++) {
    MPI_Irecv(rvalues+p,1,MPI_DOUBLE,
	      /* from */ p,/* tag */ 0,comm,requests+p);
  }
  for (p=0; p<ntids; p++) {
    double myvalue=mytid+1;
    MPI_Ssend(&myvalue,1,MPI_DOUBLE,
	      /* to */ p,/* tag */ 0,comm);
  }
  MPI_Waitall(ntids,requests,MPI_STATUSES_IGNORE);

  /*
   * Correctness test
   */
  double sum=0.;
  for (int p=0; p<ntids; p++)
    if (p!=mytid)
      sum += rvalues[p];
  double err = fabs( ntids*(ntids+1)/2.-mytid-1 - sum )/sum;
  /* printf("[%d] Compare: received=%e, s/b=%e\n", */
  /* 	 mytid,sum,ntids*(ntids+1)/2.-mytid-1); */
  if (mytid==0)
    MPI_Reduce(MPI_IN_PLACE,&err,1,MPI_DOUBLE,MPI_MAX,0,comm);
  else
    MPI_Reduce(&err,MPI_IN_PLACE,1,MPI_DOUBLE,MPI_MAX,0,comm);
  //  MPI_Allreduce(MPI_IN_PLACE,&err,1,MPI_DOUBLE,MPI_MAX,comm);
  if (mytid==0) {
    if (err>1.e-5)
      printf("Something went wrong\n");
    else
      printf("Success\n");
  }

  MPI_Finalize();
  return 0;
}
