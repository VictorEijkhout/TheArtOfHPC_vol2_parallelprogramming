/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% vectorx.c : sending large amounts of data
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

  if (nprocs<2) {
    printf("This program needs at least two processes\n");
    return -1;
  }
  int sender = 0, receiver = 1, the_other = 1-procno,
    count = 5,stride=2;

  if (procno==sender) printf("size of size_t = %d\n",sizeof(size_t));

  float *source=NULL,*target=NULL;
  int mediumsize = 1<<30;
  int nblocks = 8;
  size_t datasize = (size_t)mediumsize * nblocks * sizeof(float);

  if (procno==sender)
    printf("datasize = %lld bytes =%7.3f giga-bytes = %7.3f gfloats\n",
	   datasize,datasize*1.e-9,datasize*1.e-9/sizeof(float));

  if (procno==sender) {
    source = (float*) malloc(datasize);
    if (source) {
      printf("Source allocated\n");
    } else {
      printf("Could not allocate source data\n"); MPI_Abort(comm,1); }
    long int idx = 0;
    for (int iblock=0; iblock<nblocks; iblock++) {
      for (int element=0; element<mediumsize; element++) {
	source[idx] = idx+.5; idx++;
	if (idx<0) { printf("source index wrap\n"); MPI_Abort(comm,0); }
      }
    }
  }

  if (procno==receiver) {
    target = (float*) malloc(datasize);
    if (target) {
      printf("Target allocated\n");
    } else {
      printf("Could not allocate target data\n"); MPI_Abort(comm,1); }
  }


  MPI_Datatype blocktype;
  MPI_Type_contiguous(mediumsize,MPI_FLOAT,&blocktype);
  MPI_Type_commit(&blocktype);
  if (procno==sender) {
    MPI_Send(source,nblocks,blocktype,receiver,0,comm);
  } else if (procno==receiver) {
    MPI_Status recv_status;
    MPI_Recv(target,nblocks,blocktype,sender,0,comm,
      &recv_status);
    MPI_Count recv_count;
    MPI_Get_elements_x(&recv_status,MPI_FLOAT,&recv_count);
    printf("Received %7.3f medium size elements\n",recv_count * 1e-9);
  }
  MPI_Type_free(&blocktype);
  
  if (0 && procno==receiver) {
    for (int i=0; i<count; i++)
      if (target[i]!=source[stride*i])
	printf("location %d %e s/b %e\n",i,target[i],source[stride*i]);
  }

  if (procno==0)
    printf("Finished\n");

  if (procno==sender)
    free(source);
  if (procno==receiver)
    free(target);

  MPI_Finalize();
  return 0;
}
