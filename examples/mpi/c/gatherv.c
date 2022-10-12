/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% gatherv.c : MPI_Gatherv
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

  int *localsizes=NULL,*offsets=NULL,*localdata=NULL,*alldata=NULL;
  // create local data
  localdata = (int*) malloc( localsize*sizeof(int) );
  for (int i=0; i<localsize; i++)
    localdata[i] = procno+1;
  // we assume that each process has an array "localdata"
  // of size "localsize"

  // the root process decides how much data will be coming:
  // allocate arrays to contain size and offset information
  if (procno==root) {
    localsizes = (int*) malloc( nprocs*sizeof(int) );
    offsets = (int*) malloc( nprocs*sizeof(int) );
  }
  // everyone contributes their local size info
  MPI_Gather(&localsize,1,MPI_INT,
	     localsizes,1,MPI_INT,root,comm);

  if (procno==root) {
    printf("Local sizes: ");
    for (int i=0; i<nprocs; i++)
      printf("%d, ",localsizes[i]);
    printf("\n");
  }

  // the root constructs the offsets array
  if (procno==root) {
    int total_data = 0;
    for (int i=0; i<nprocs; i++) {
      offsets[i] = total_data;
      total_data += localsizes[i];
    }
    alldata = (int*) malloc( total_data*sizeof(int) );
  }
  // everyone contributes their data
  MPI_Gatherv(localdata,localsize,MPI_INT,
	      alldata,localsizes,offsets,MPI_INT,root,comm);

  if (procno==root) {
    int p=0;
    printf("Collected:\n");
    for (int i=0; i<nprocs; i++) {
      int j;
      printf("  %d:",i);
      for (j=0; j<localsizes[i]-1; j++)
	printf("%d,",alldata[p++]);
      j=localsizes[i]-1;
      printf("%d;\n",alldata[p++]);
    }
  }

  MPI_Finalize();
  return 0;
}
