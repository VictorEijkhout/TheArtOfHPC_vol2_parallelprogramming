/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% countx.c : sending really large amount of data
   %%%% IS THIS SUPERFLUOUS? see vectorx.c
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

  if (nprocs<2) {
    printf("This program needs at least two processes\n");
    return -1;
  }

  int sender = 0, receiver = nprocs-1;

  // send million elements, so typesize == megawords
#define SENDCOUNT 1000000
  for (int itest=0,typesize=10; itest<4; itest++,typesize*=10) {
    if ( procno!=sender && procno!=receiver )
      continue;
    int megawords = typesize;
    double *source_data,*target_data;
    long int buffer_size_in_words = SENDCOUNT * typesize;
    long int buffer_size_in_bytes = buffer_size_in_words * sizeof(double);
    if (procno==sender) 
      printf("Experiment with count=%10ld, typesize=%10ld, words=%12ld, megawords=%10ld, msg size in bytes=%12ld\n",
	     SENDCOUNT,typesize,buffer_size_in_words,megawords,buffer_size_in_bytes);
    if (procno==sender) {
      source_data = (double*) malloc(buffer_size_in_bytes);
      if (!source_data) {
	printf("[%d] could not allocate source data\n",procno);
	MPI_Abort(comm,0); }
      memset(source_data,1,buffer_size_in_bytes);
    }
    if (procno==receiver) {
      target_data = (double*) malloc(buffer_size_in_bytes);
      if (!target_data) {
	printf("[%d] could not allocate target data\n",procno);
	MPI_Abort(comm,0); }
      memset(target_data,1,buffer_size_in_bytes);
    }

    MPI_Datatype newvectortype;
    MPI_Type_contiguous(typesize,MPI_DOUBLE,&newvectortype);
    MPI_Type_commit(&newvectortype);
    if (procno==sender) {
      MPI_Ssend(source_data,SENDCOUNT,newvectortype,receiver,0,comm);
    } else if (procno==receiver) {
      MPI_Status recv_status;
      int type_count;
      MPI_Recv(target_data,SENDCOUNT,newvectortype,sender,0,comm,
	       &recv_status);
      MPI_Get_count(&recv_status,newvectortype,&type_count);
      ASSERT(SENDCOUNT==type_count);
      MPI_Count element_count;
      // get_elements_x doesn't care what type you give?
      MPI_Get_elements_x(&recv_status,MPI_DOUBLE,&element_count);
      fprintf(stderr,"on receive counting %ld doubles\n",element_count);
      ASSERT(element_count==buffer_size_in_words);
    }
    MPI_Type_free(&newvectortype);
    free(source_data); free(target_data);
  }

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
