/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% gather.c : illustrating MPI_Gather
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
  int localsize =  10+10*( (float) rand()/(float)RAND_MAX - .5),
    root = nprocs-1;

  int *localsizes=NULL;
  // create local data
  int *localdata = (int*) malloc( localsize*sizeof(int) );
  for (int i=0; i<localsize; i++)
    localdata[i] = procno+1;
  // we assume that each process has a value "localsize"
  // the root process collects these values

  if (procno==root)
    localsizes = (int*) malloc( nprocs*sizeof(int) );

  // everyone contributes their info
  MPI_Gather(&localsize,1,MPI_INT,
             localsizes,1,MPI_INT,root,comm);
  if (procno==root) {
    printf("Local sizes: ");
    for (int i=0; i<nprocs; i++)
      printf("%d, ",localsizes[i]);
    printf("\n");
  }

  MPI_Finalize();
  return 0;
}
