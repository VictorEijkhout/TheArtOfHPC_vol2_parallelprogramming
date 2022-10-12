/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2022
   %%%%
   %%%% partition.c : partitioned communication
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

void fill_buffer(double *send,int f,int l,int n) {
  for (int i=f; i<l; i++)
    send[i] = n;
}
int chck_buffer(double *recv,int f,int l,int n) {
  int r = 1;
  for (int i=f; i<l; i++)
    r = r && recv[i]==n;
  return r;
}

void report_time(double t,int s,int p) {
  printf("Time for buffer size %d, partitioned %d times: %e per pingpong\n",s,p,t);
}

int main(int argc,char **argv) {

#include "globalinit.c"

#define ITERATIONS 100
#define SIZE 1000
  int src = 0,tgt = nprocs-1,maxsize=1000000;

  // Now persistent communication
  for (int nparts=1; nparts<=40; nparts++ ) {
    int bufsize = nparts*SIZE;
    int *partitions = (int*)malloc((nparts+1)*sizeof(int));
    for (int ip=0; ip<=nparts; ip++)
      partitions[ip] = ip*SIZE;
    if (procno==src) {
      double *sendbuffer = (double*)malloc(bufsize*sizeof(double));
      fill_buffer(sendbuffer,0,bufsize,1);
      double start = MPI_Wtime();
      MPI_Request send_request;
      MPI_Psend_init
        (sendbuffer,nparts,SIZE,MPI_DOUBLE,tgt,0,
         comm,MPI_INFO_NULL,&send_request);
      for (int it=0; it<ITERATIONS; it++) {
        MPI_Start(&send_request);
        for (int ip=0; ip<nparts; ip++) {
          fill_buffer(sendbuffer,partitions[ip],partitions[ip+1],ip);
          MPI_Pready(ip,send_request);
	}
        MPI_Wait(&send_request,MPI_STATUS_IGNORE);
        int confirm; MPI_Recv(&confirm,0,MPI_INT,tgt,0,comm,MPI_STATUS_IGNORE);
      }
      MPI_Request_free(&send_request);
      double duration = MPI_Wtime() - start;
      report_time(duration/ITERATIONS,bufsize,nparts);
      free(sendbuffer);
    } else if (procno==tgt) {
      double *recvbuffer = (double*)malloc(bufsize*sizeof(double));
      MPI_Request recv_request;
      MPI_Precv_init
        (recvbuffer,nparts,SIZE,MPI_DOUBLE,src,0,
         comm,MPI_INFO_NULL,&recv_request);
      for (int it=0; it<ITERATIONS; it++) {
        MPI_Start(&recv_request); int r=1,flag;
        for (int ip=0; ip<nparts; ip++) // cycle this many times
          for (int ap=0; ap<nparts; ap++) { // check specific part
            MPI_Parrived(recv_request,ap,&flag);
            if (flag) {
              r *= chck_buffer
                (recvbuffer,partitions[ap],partitions[ap+1],ap);
              break; }
          }
        MPI_Wait(&recv_request,MPI_STATUS_IGNORE);
        if (!r) printf("buffer problem %d\n",1);
        int confirm; MPI_Send(&confirm,0,MPI_INT,src,0,comm);
      }
      MPI_Request_free(&recv_request);
      free(recvbuffer);
    }
    free(partitions);
  }

  MPI_Finalize();
  return 0;
}
