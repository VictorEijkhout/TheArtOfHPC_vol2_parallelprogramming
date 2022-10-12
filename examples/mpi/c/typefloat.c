/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** MPI_Datatype illustration
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Init(&argc,&argv);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  int processA,processB;
  processA = 0; processB = nprocs-1;
#ifdef DOUBLE
  MPI_Datatype buffertype = MPI_DOUBLE;
  double send_buffer,recv_buffer; 
#else
  MPI_Datatype buffertype = MPI_FLOAT;
  float send_buffer,recv_buffer; 
#endif
  send_buffer = 1.1;
  if (procno==processA) {
      MPI_Send(&send_buffer,1,buffertype,
	       processB,0,
	       comm);
  } else if (procno==processB) {
      MPI_Recv(&recv_buffer,1,buffertype,
	       processA,0,
	       comm,MPI_STATUS_IGNORE);
  }

  MPI_Finalize();
  return 0;
}
