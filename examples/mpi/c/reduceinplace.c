/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% reduceinplace.c : reduction in place
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

  float mynumber,result,*sendbuf,*recvbuf;
  mynumber = (float) procno;
  int target_proc = nprocs-1;
  // add all the random variables together
  if (procno==target_proc) {
    sendbuf = (float*)MPI_IN_PLACE; recvbuf = &result;
    result = mynumber;
  } else {
    sendbuf = &mynumber;    recvbuf = NULL;
  }
  MPI_Reduce(sendbuf,recvbuf,1,MPI_FLOAT,MPI_SUM,
             target_proc,comm);
  // the result should be nprocs*(nprocs-1)/2:
  if (procno==target_proc)
    printf("Result %6.3f compared to n(n-1)/2=%5.2f\n",
           result,nprocs*(nprocs-1)/2.);

  MPI_Finalize();
  return 0;
}
