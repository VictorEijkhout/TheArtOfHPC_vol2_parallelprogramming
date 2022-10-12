/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

int function_start(MPI_Comm comm,void **ctx);
int function_end(MPI_Comm comm,void *ctx);

int main(int argc,char **argv) {

#include "globalinit.c"

  int left,right, sdata=5,rdata, wrong=0;
  MPI_Request request[2];
  MPI_Status status[2];
  void *ctx;
  left = procno-1; if (left<0) left = MPI_PROC_NULL;
  right = procno+1; if (right>=nprocs) right = MPI_PROC_NULL;
  for (int try=0; try<1000; try++) {
    ierr = MPI_Isend(&sdata,1,MPI_INT,right,1,comm,&(request[0])); CHK(ierr);
    function_start(comm,&ctx);
    ierr = MPI_Irecv(&rdata,1,MPI_INT,left,MPI_ANY_TAG,comm,&(request[1])); CHK(ierr);
    ierr = MPI_Waitall(2,request,status); CHK(ierr);
    function_end(comm,ctx);
    if (status[1].MPI_TAG==2 || status[1].MPI_SOURCE!=left)
      wrong++;
  }
  printf("#wrong: %d\n",wrong);
 skip:
  MPI_Finalize();
  return 0;
}

int function_start(MPI_Comm comm,void **ctx) {
  int procno,nprocs,left,right, sdata=6,rdata, ierr;
  MPI_Request *request;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  left = procno-1; if (left<0) left = MPI_PROC_NULL;
  right = procno+1; if (right>=nprocs) right = MPI_PROC_NULL;

  request = (MPI_Request*) malloc(2*sizeof(MPI_Request)); *ctx = (void*)request;

  ierr = MPI_Isend(&sdata,1,MPI_INT,right,2,comm,&(request[0])); CHK(ierr);
  ierr = MPI_Irecv(&rdata,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,comm,&(request[1])); CHK(ierr);
  return 0;
}

int function_end(MPI_Comm comm,void *ctx) {
  MPI_Request *request = (MPI_Request*)ctx;
  MPI_Status status[2];
  int procno,nprocs,ierr;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);
  ierr = MPI_Waitall(2,request,status); CHK(ierr);
  free(request);
  return 0;
}

