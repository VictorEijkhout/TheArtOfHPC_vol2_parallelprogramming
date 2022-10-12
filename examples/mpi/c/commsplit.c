/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% commsplit.c : illustrating MPI_Comm_split
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

  if (nprocs<3) {
    printf("This program needs at least three processes\n");
    return -1;
  }

  int color;
  int new_procno;
  MPI_Comm mod2comm,mod4comm;

  int mydata = procno;
  // create sub communicator modulo 2
  color = procno%2;
  MPI_Comm_split(MPI_COMM_WORLD,color,procno,&mod2comm);
  MPI_Comm_rank(mod2comm,&new_procno);

  // create sub communicator modulo 4 recursively
  color = new_procno%2;
  MPI_Comm_split(mod2comm,color,new_procno,&mod4comm);
  MPI_Comm_rank(mod4comm,&new_procno);

  if (mydata/4!=new_procno)
    printf("Error %d %d %d\n",procno,new_procno,mydata/4);

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}

