/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% pack.c : use pack and unpack to send data
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
  if (argc<2) {
    printf("Usage: %s nnn\n",argv[0]);
    return -2;
  }
  if (procno==0) {
    printf("Size\n");
    int s;
    for (int i=1; i<=4; i++) {
      MPI_Pack_size(i,MPI_CHAR,comm,&s);
      printf("%d chars: %d\n",i,s);
    }
    for (int i=1; i<=4; i++) {
      MPI_Pack_size(i,MPI_UNSIGNED_SHORT,comm,&s);
      printf("%d unsigned shorts: %d\n",i,s);
    }
    for (int i=1; i<=4; i++) {
      MPI_Pack_size(i,MPI_INT,comm,&s);
      printf("%d ints: %d\n",i,s);
    }
    printf("size\n");
  }

  int sender=0,receiver=nprocs-1,other=nprocs-1-procno;
  int nsends=atoi(argv[1]),maxsends=100;
  if (nsends>maxsends) {
    printf("Max value for commandline argument: %d\n",maxsends);
    return -3;
  }
  int buflen = 1000,
    position;
  char *buffer = malloc(buflen);
  if (procno==0) printf("Packing\n");
  if (procno==sender) {
    int len=2*sizeof(int)+nsends*sizeof(double)+MPI_BSEND_OVERHEAD;
    if (len>buflen) {
      printf("Not enough buffer space, need %d\n",len);
      return -4;
    }
    position = 0;
    MPI_Pack(&nsends,1,MPI_INT,
             buffer,buflen,&position,comm);
    for (int i=0; i<nsends; i++) {
      double value = rand()/(double)RAND_MAX;
      printf("[%d] pack %e\n",procno,value);
      MPI_Pack(&value,1,MPI_DOUBLE,
               buffer,buflen,&position,comm);
    }
    MPI_Pack(&nsends,1,MPI_INT,
             buffer,buflen,&position,comm);
    MPI_Send(buffer,position,MPI_PACKED,other,0,comm);
  } else if (procno==receiver) {
    int irecv_value;
    double xrecv_value;
    MPI_Recv(buffer,buflen,MPI_PACKED,other,0,
             comm,MPI_STATUS_IGNORE);
    position = 0;
    MPI_Unpack(buffer,buflen,&position,
               &nsends,1,MPI_INT,comm);
    for (int i=0; i<nsends; i++) {
      MPI_Unpack(buffer,buflen,
                 &position,&xrecv_value,1,MPI_DOUBLE,comm);
      printf("[%d] unpack %e\n",procno,xrecv_value);
    }
    MPI_Unpack(buffer,buflen,&position,
               &irecv_value,1,MPI_INT,comm);
    ASSERT(irecv_value==nsends);
  }
  if (procno==0) printf("packing\n");

  MPI_Finalize();
  return 0;
}
