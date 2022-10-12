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

  MPI_Comm comm;
  int procno=-1,nprocs,err;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  /*
   * Set up a process ring.
   * This would deadlock with ordinary MPI_Send/MPI_Recv.
   */
  int
    next = (procno+1) % nprocs,
    prev = (procno-1+nprocs) % nprocs;
  printf("[%d] to: %d, from: %d\n",procno,next,prev);

#define BUFLEN 10000
  int bsize = BUFLEN*sizeof(float);
  float
    *sbuf = (float*) malloc( bsize ),
    *rbuf = (float*) malloc( bsize );
  MPI_Pack_size( BUFLEN,MPI_FLOAT,comm,&bsize);
  bsize += MPI_BSEND_OVERHEAD;
  float
    *buffer = (float*) malloc( bsize );

  MPI_Buffer_attach( buffer,bsize );
  err = MPI_Bsend(sbuf,BUFLEN,MPI_FLOAT,next,0,comm);
  MPI_Recv (rbuf,BUFLEN,MPI_FLOAT,prev,0,comm,MPI_STATUS_IGNORE);
  MPI_Buffer_detach( &buffer,&bsize );

  printf("Done on %d\n",procno);
  
  MPI_Finalize();
  return 0;
}
