/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% persist.c : test of persistent communication
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

void fill_buffer(double *send,int s,int n) {
  for (int i=0; i<s; i++)
    send[i] = n;
}
int chck_buffer(double *recv,int s,int n) {
  int r = 1;
  for (int i=0; i<s; i++)
    r = r && recv[i]==n;
  return r;
}

int main(int argc,char **argv) {

#include "globalinit.c"

#define NEXPERIMENTS 10
#define NSIZES 6
  int src = 0,tgt = nprocs-1,maxsize=1000000;
  double t[NSIZES], send[maxsize],recv[maxsize];

  MPI_Request requests[2];

  // First ordinary communication
  for (int cnt=0,s=1; cnt<NSIZES && s<maxsize; s*=10,cnt++) {
    if (procno==src) {
      t[cnt] = MPI_Wtime();//snipthis
      for (int n=0; n<NEXPERIMENTS; n++) {
        MPI_Isend(send,s,MPI_DOUBLE,tgt,0,comm,requests+0);
        MPI_Irecv(recv,s,MPI_DOUBLE,tgt,0,comm,requests+1);
        MPI_Waitall(2,requests,MPI_STATUSES_IGNORE);
      }
      t[cnt] = MPI_Wtime()-t[cnt];
    } else if (procno==tgt) {
      for (int n=0; n<NEXPERIMENTS; n++) {
        MPI_Recv(recv,s,MPI_DOUBLE,src,0,comm,MPI_STATUS_IGNORE);
        MPI_Send(recv,s,MPI_DOUBLE,src,0,comm);
      }
    }
  }
  if (procno==src) {
    for (int cnt=0,s=1; cnt<NSIZES; s*=10,cnt++) {
      t[cnt] /= NEXPERIMENTS;
      printf("Pingpong size=%d: t=%7.4e\n",s,t[cnt]);
    }
  }

  // Now persistent communication
  for (int cnt=0,s=1; cnt<NSIZES; s*=10,cnt++) {
    if (procno==src) {
      MPI_Send_init
        (send,s,MPI_DOUBLE,tgt,0,comm,
         requests+0);
      MPI_Recv_init
        (recv,s,MPI_DOUBLE,tgt,0,comm,
         requests+1);
      t[cnt] = MPI_Wtime();//snipthis
      for (int n=0; n<NEXPERIMENTS; n++) {
        fill_buffer(send,s,n);
        MPI_Startall(2,requests);
        MPI_Waitall(2,requests,
                    MPI_STATUSES_IGNORE);
        int r = chck_buffer(send,s,n);
        if (!r) printf("buffer problem %d\n",s);
      }
      t[cnt] = MPI_Wtime()-t[cnt];//snipthis
      MPI_Request_free(requests+0);
      MPI_Request_free(requests+1);
    } else if (procno==tgt) {
      for (int n=0; n<NEXPERIMENTS; n++) {
        MPI_Recv(recv,s,MPI_DOUBLE,src,0,
                comm,MPI_STATUS_IGNORE);
        MPI_Send(recv,s,MPI_DOUBLE,src,0,
                 comm);
      }
    }
  }
  if (procno==src) {
    for (int cnt=0,s=1; cnt<NSIZES; s*=10,cnt++) {
      t[cnt] /= NEXPERIMENTS;
      printf("Persistent size=%d: t=%7.4e\n",s,t[cnt]);
    }
  }

  MPI_Finalize();
  return 0;
}
