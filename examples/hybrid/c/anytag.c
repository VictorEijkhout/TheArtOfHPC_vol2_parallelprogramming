/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2022
 ****
 **** anytag.c : MPI_ANY_TAG wildcard
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "threadinit.c"

  /*
   * We set up a single communication between 
   * the first and last process
   */
  int sender,receiver;
  sender = 0; receiver = nprocs-1;

  int reversed=0;
  for (int iexperiment=0; iexperiment<10000; iexperiment++ ) {
  MPI_Request requests[2];
  int xtag=1, ytag=2;
  double x=3.14, y=2.81;
  if (procno==sender) {
    double send_data = 1.;
#pragma omp parallel sections
    {
#pragma omp section
    MPI_Isend
      ( &x,1,MPI_DOUBLE,
	receiver,xtag,comm,requests+0);
#pragma omp section
    MPI_Isend
      ( &y,1,MPI_DOUBLE,
	receiver,ytag,comm,requests+1);
    }
    MPI_Waitall(2,requests,MPI_STATUSES_IGNORE);
  } else if (procno==receiver) {
    double xy1,xy2;
    MPI_Status statuses[2];
#pragma omp parallel sections
    {
#pragma omp section
    MPI_Irecv
      ( &xy1,1,MPI_DOUBLE,
	sender, MPI_ANY_TAG, comm, requests+0);
#pragma omp section
    MPI_Irecv
      ( &xy2,1,MPI_DOUBLE,
	sender, MPI_ANY_TAG, comm, requests+1);
    }
    MPI_Waitall(2,requests,statuses);
    if ( statuses[0].MPI_TAG==xtag ) {
      x = xy1; y = xy2;
    } else { reversed++;
      y = xy1; x = xy2;
    }
  }
  }
  if (procno==receiver)
    printf("reversed: %d\n",reversed);

  MPI_Finalize();
  return 0;
}
