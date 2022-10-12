/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** sendandrecv.c : Simple illustration of send and recv
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

  /*
   * We set up a single communication between 
   * the first and last process
   */
  int sender,receiver;
  sender = 0; receiver = nprocs-1;

  if (procno==sender) {
    double send_data = 1.;
    MPI_Send
      ( /* send buffer/count/type: */ &send_data,1,MPI_DOUBLE,
        /* to: */ receiver, /* tag: */ 0,
        /* communicator: */ comm);
    printf("[%d] Send successfully concluded\n",procno);
  } else if (procno==receiver) {
    double recv_data;
    MPI_Recv
      ( /* recv buffer/count/type: */ &recv_data,1,MPI_DOUBLE,
        /* from: */ sender, /* tag: */ 0,
        /* communicator: */ comm,
        /* recv status: */ MPI_STATUS_IGNORE);
    printf("[%d] Receive successfully concluded\n",procno);
  }

  MPI_Finalize();
  return 0;
}
