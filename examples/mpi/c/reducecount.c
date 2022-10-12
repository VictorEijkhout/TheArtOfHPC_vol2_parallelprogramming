/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% reducecount.c : use of allreduce_c and MPI_Count
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc,char **argv) {
  MPI_Comm comm;
  int ntids,mytid;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&ntids);
  MPI_Comm_rank(comm,&mytid);

  if (mytid==0)
    printf("MPI_count: %ld, size_t: %ld\n",sizeof(MPI_Count),sizeof(size_t));

  MPI_Count buffersize = 1000;
  double *indata,*outdata;
  indata = (double*) malloc( buffersize*sizeof(double) );
  outdata = (double*) malloc( buffersize*sizeof(double) );

  for (size_t i=0; i<buffersize; i++) {
    indata[i] = mytid; outdata[i] = 0;
  }

  MPI_Allreduce_c(indata,outdata,buffersize,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
  if (mytid==0) printf("sum: %5.1f\n",outdata[0]);

  MPI_Finalize();
  return 0;
}

