/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% MPI example for resized vector type & interleaved gather
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm; int nprocs,procno;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  int root = nprocs-1, localsize = 4;

  int
    *mydata = (int*) malloc( localsize*sizeof(int) ),
    *gathered;
  if (!mydata ) {
      printf("Out of memory\n"); MPI_Abort(comm,0); }
  for (int i=0; i<localsize; i++)
    mydata[i] = i*nprocs+procno;
  if (procno==root) {
    gathered = (int*) malloc( localsize*nprocs*sizeof(int) );
    if (!gathered ) {
      printf("Out of memory\n"); MPI_Abort(comm,0); }
  }

  /*
   * Datatype for strided destinations
   */
  MPI_Datatype stridetype;
  int count,stride,blocklength;
  count = localsize; stride = nprocs; blocklength = 1;
  MPI_Type_vector(count,blocklength,stride,MPI_INT,&stridetype);
  MPI_Type_commit(&stridetype);
  if (procno==root) {
    MPI_Aint l,e;
    MPI_Type_get_extent(stridetype,&l,&e);
    printf("Stride type l=%ld e=%ld\n",l,e);
  }

  MPI_Datatype interleavetype;
  MPI_Type_create_resized(stridetype,0,sizeof(int),&interleavetype);
  MPI_Type_commit(&interleavetype);
  if (procno==root) {
    MPI_Aint l,e;
    MPI_Type_get_extent(interleavetype,&l,&e);
    printf("Interleave type l=%ld e=%ld\n",l,e);
  }

#if 0
  MPI_Gather( mydata,localsize,MPI_INT,
	      gathered,localsize,MPI_INT, // abutting
	      root,comm );
  MPI_Gather( mydata,localsize,MPI_INT,
	      gathered,1,stridetype, // abut with gaps
	      root,comm );
#endif
  MPI_Gather( mydata,localsize,MPI_INT,
	      gathered,1,interleavetype, // shrunk extent
	      root,comm );
  if (procno==root) {
    printf("gather %d elements from %d procs:\n",localsize,nprocs);
    for (int i=0; i<nprocs*localsize; i++)
      printf(" %d",gathered[i]);
    printf("\n");
  }

  MPI_Type_free(&stridetype);
  MPI_Type_free(&interleavetype);

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
