/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
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

  int nrandoms = 500000;
  float *myrandoms;
  myrandoms = (float*) malloc(nrandoms*sizeof(float));
  for (int iter=1; iter<=3; iter++) {
    /*
     * We show three different ways of doing the same reduction;
     * this illustrates syntax more than semantics
     */
    if (iter==1) {
      for (int irand=0; irand<nrandoms; irand++)
	myrandoms[irand] = (float) rand()/(float)RAND_MAX;
      // add all the random variables together
      MPI_Allreduce(MPI_IN_PLACE,myrandoms,
		    nrandoms,MPI_FLOAT,MPI_SUM,comm);
    } else if (iter==2) {
      for (int irand=0; irand<nrandoms; irand++)
	myrandoms[irand] = (float) rand()/(float)RAND_MAX;
      int root=nprocs-1;
      if (procno==root)
        MPI_Reduce(MPI_IN_PLACE,myrandoms,
		   nrandoms,MPI_FLOAT,MPI_SUM,root,comm);
      else
        MPI_Reduce(myrandoms,MPI_IN_PLACE,
		   nrandoms,MPI_FLOAT,MPI_SUM,root,comm);
    } else if (iter==2) {
      for (int irand=0; irand<nrandoms; irand++)
	myrandoms[irand] = (float) rand()/(float)RAND_MAX;
      int root=nprocs-1;
      float *sendbuf,*recvbuf;
      if (procno==root) {
        sendbuf = MPI_IN_PLACE; recvbuf = myrandoms;
      } else {
        sendbuf = myrandoms; recvbuf = MPI_IN_PLACE;
      }
      MPI_Reduce(sendbuf,recvbuf,
        	 nrandoms,MPI_FLOAT,MPI_SUM,root,comm);
    }
    // the result should be approx nprocs/2:
    if (procno==nprocs-1) {
      float sum=0.;
      for (int i=0; i<nrandoms; i++) sum += myrandoms[i];
      sum /= nrandoms*nprocs;
      printf("Result %6.9f compared to .5\n",sum);
    }
  }
  free(myrandoms);

  MPI_Finalize();
  return 0;
}
