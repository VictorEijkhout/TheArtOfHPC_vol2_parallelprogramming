/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% stridestretch.c : vector datatype
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
  int sender = 0, receiver = 1, the_other = 1-procno,
    count = 5,stride=2;
  double *source,*target;
  source = (double*) malloc(stride*count*sizeof(double));
  target = (double*) malloc(stride*count*sizeof(double));

  for (int i=0; i<stride*count; i++)
    source[i] = i+.5;

  if (procno==sender) {
    MPI_Datatype oneblock;
    MPI_Type_vector(1,1,stride,MPI_DOUBLE,&oneblock);
    MPI_Type_commit(&oneblock);
    MPI_Aint block_lb,block_x;
    MPI_Type_get_extent(oneblock,&block_lb,&block_x);
    printf("One block has extent: %ld\n",block_x);

    MPI_Datatype paddedblock;
    MPI_Type_create_resized(oneblock,0,stride*sizeof(double),&paddedblock);
    MPI_Type_commit(&paddedblock);
    MPI_Type_get_extent(paddedblock,&block_lb,&block_x);
    printf("Padded block has extent: %ld\n",block_x);

    // now send a bunch of these padded blocks
    MPI_Send(source,count,paddedblock,the_other,0,comm);
    MPI_Type_free(&oneblock);
    MPI_Type_free(&paddedblock);
  } else if (procno==receiver) {
    int blens[2]; MPI_Aint displs[2];
    MPI_Datatype types[2], paddedblock;
    blens[0] = 1; blens[1] = 1;
    displs[0] = 0; displs[1] = 2 * sizeof(double);
    types[0] = MPI_DOUBLE; types[1] = MPI_UB;
    MPI_Type_struct(2, blens, displs, types, &paddedblock);
    MPI_Type_commit(&paddedblock);
    MPI_Status recv_status;
    MPI_Recv(target,count,paddedblock,the_other,0,comm,&recv_status);
    /* MPI_Recv(target,count,MPI_DOUBLE,the_other,0,comm, */
    /*   &recv_status); */
    int recv_count;
    MPI_Get_count(&recv_status,MPI_DOUBLE,&recv_count);
    ASSERT(recv_count==count);
  }
  
  if (procno==receiver) {
    for (int i=0; i<count; i++)
      if (target[i*stride]!=source[i*stride])
	printf("location %d %e s/b %e\n",i,target[i*stride],source[stride*i]);
  }

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
