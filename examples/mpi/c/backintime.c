/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-8
   %%%%
   %%%% backintime.c : illustrating temporal interleaving
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
    if (procno==0)
      printf("Need at least 3 processes\n");
    MPI_Abort(comm,3);
  }
  
  MPI_Status statusa,statusb;
  double mydata = 1.5,gdata, forone = 3.2,froma,fromb;
  int root = 1;

  switch (procno) {
  case 0 :
    MPI_Reduce(&mydata,&gdata,1,MPI_DOUBLE,MPI_MAX,root,comm);
    //MPI_Barrier(comm);
    MPI_Send(&forone,1,MPI_DOUBLE,/* dest: */ 1,0,comm);
    break;
  case 1 :
    MPI_Recv(&froma,1,MPI_DOUBLE,MPI_ANY_SOURCE,0,comm,&statusa);
    //MPI_Barrier(comm);
    MPI_Reduce(&mydata,&gdata,1,MPI_DOUBLE,MPI_MAX,root,comm);
    MPI_Recv(&fromb,1,MPI_DOUBLE,MPI_ANY_SOURCE,0,comm,&statusb);
    break;
  case 2 :
    MPI_Send(&forone,1,MPI_DOUBLE,/* dest: */ 1,0,comm);
    //MPI_Barrier(comm);
    MPI_Reduce(&mydata,&gdata,1,MPI_DOUBLE,MPI_MAX,root,comm);
    break;
  }
  if (procno==1) {
    if (statusa.MPI_SOURCE==0) {
      printf("back in time!\n");
      if (statusb.MPI_SOURCE!=2)
	printf("(weirdness in second recv: src=%d)\n",statusb.MPI_SOURCE);
    } else if (statusa.MPI_SOURCE==2) {
      printf("logical...\n");
      if (statusb.MPI_SOURCE!=0)
	printf("(weirdness in second recv: src=%d)\n",statusb.MPI_SOURCE);
    } else
      printf("ERROR unexpected sources: %d,%d\n",statusa.MPI_SOURCE,statusb.MPI_SOURCE);
  }

  MPI_Finalize();
  return 0;
}
