/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2019-2022
 ****
 **** MPI Exercise to illustrate deadlock
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "mpi.h"

void do_some_work() {}

int main(int argc,char **argv) {

#include "globalinit.c"

  double
    mydata=procno;
  int sender = nprocs-1;

  if (procno==sender) {
    for (int p=0; p<nprocs-1; p++) {
      double send = 1.;
      MPI_Send( &send,1,MPI_DOUBLE,p,0,comm);
    }
  } else {
    double recv=0.;
    MPI_Request request;
    MPI_Irecv( &recv,1,MPI_DOUBLE,sender,0,comm,&request);
    do_some_work();
    MPI_Wait(&request,MPI_STATUS_IGNORE);
  }

  MPI_Finalize();
  return 0;
}
