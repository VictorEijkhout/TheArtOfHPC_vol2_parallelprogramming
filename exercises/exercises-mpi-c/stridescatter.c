/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% MPI example for vector type and extent resizing
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm; 
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  int nprocs,procno;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  if (nprocs<2) {
    printf("This program needs at least two processes\n");
    return -1;
  }
  int sender = 0, localsize = 3;

  // big data array, will only be created on the root
  int *data = NULL;

  // datatypes, will only be used on the root
  MPI_Datatype scattertype,interleavetype;

  if (procno==sender) {

    /*
     * Create big data array to be sent
     */
    int ndata = localsize*nprocs;
    data = (int*) malloc(ndata*sizeof(int));
    if (!data) {
      printf("Out of memory\n"); MPI_Abort(comm,0); }
    for (int i=0; i<ndata; i++)
      data[i] = i;

    /*
     * Exercise
     * - you need a vector datatype for sending data.
     * - resize its extent to achieve proper interleaving
     */
    int count,stride,blocklength;
    MPI_Aint l,e;
    count = localsize; stride = nprocs; blocklength = 1;
    MPI_Type_vector(count,blocklength,stride,MPI_INT,&scattertype);
    MPI_Type_commit(&scattertype);
/**** your code here ****/
  }

  // all processes have local data:
  int *mydata = (int*) malloc(localsize*sizeof(int));
  MPI_Scatter( /* send: */ data,1,interleavetype,
	       /* recv: */ mydata,localsize,MPI_INT,
	       sender,comm
	       );


#if 0
  /*
   * Trace output is disabled by default;
   * change the 0 to 1 to get a full printout
   * of the data of each process
   */
  printf("[%2d]",procno);
  for (int i=0; i<localsize; i++)
    printf(" %2d",mydata[i]);
  printf("\n");
#endif
  
  /*
   * Correctness checking
   */
  for (int i=0; i<localsize; i++)
    if (mydata[i]%nprocs!=procno)
      printf("[%d] received element=%d, should be %d\n",procno,mydata[i],i*nprocs+procno);

  if (procno==0)
    printf("Finished\n");

  if (procno==sender) {
    MPI_Type_free(&interleavetype);
    MPI_Type_free(&scattertype);
    free(data);
  }
  
  MPI_Finalize();
  return 0;
}
