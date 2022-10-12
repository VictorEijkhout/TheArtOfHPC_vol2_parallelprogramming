/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% trueextent.c : extent measuring of subarray datatype
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
  int sender = 0, receiver = 1, the_other = 1-procno;
  int sizes[2] = {4,6},subsizes[2] = {2,3},starts[2] = {1,2};
  int
    block = sizes[0]*sizes[1],
    count = subsizes[0]*subsizes[1];
  double *source,*target;
  source = (double*) malloc(block*sizeof(double));
  target = (double*) malloc(count*sizeof(double));

  for (int i=0; i<sizes[0]*sizes[1]; i++)
    source[i] = i+.5;
  
  if (procno==sender) {
    printf("In basic array of %lu bytes\n",block*sizeof(double));
    printf("find sub array of %lu bytes\n",count*sizeof(double));
    MPI_Datatype subarraytype;
    MPI_Type_create_subarray
      (2,sizes,subsizes,starts,
       MPI_ORDER_C,MPI_DOUBLE,&subarraytype);
    MPI_Type_commit(&subarraytype);

    MPI_Aint true_lb,true_extent,extent;
     MPI_Type_get_true_extent
      (subarraytype,&true_lb,&true_extent);
    MPI_Aint
      comp_lb = sizeof(double) *
          ( starts[0]*sizes[1]+starts[1] ),
      comp_extent = sizeof(double) *
          ( sizes[1]-starts[1] // first row
            + starts[1]+subsizes[1] // last row
            + ( subsizes[0]>1 ? subsizes[0]-2 : 0 )*sizes[1] );
    printf("Found lb=%ld, extent=%ld\n",true_lb,true_extent);
    printf("Computing lb=%ld extent=%ld\n",comp_lb,comp_extent);
    ASSERT(true_lb==comp_lb);
    ASSERT(true_extent==comp_extent);
    MPI_Send(source,1,subarraytype,the_other,0,comm);
    {
      MPI_Aint lb,extent,
        comp_extent = sizeof(double) *sizes[0]*sizes[1];
      MPI_Type_get_extent(subarraytype,&lb,&extent);
      printf("Non-true lb=%ld, extent=%ld, computed=%ld\n",
             lb,extent,comp_extent);
      ASSERT(extent==comp_extent);
    }
    MPI_Type_free(&subarraytype);
 } else if (procno==receiver) {
    MPI_Status recv_status;
    int recv_count;
    MPI_Recv(target,count,MPI_DOUBLE,the_other,0,comm,
      &recv_status);
    MPI_Get_count(&recv_status,MPI_DOUBLE,&recv_count);
    ASSERT(recv_count==count);
  }

  if (procno==receiver) {
    printf("received:");
    for (int i=0; i<count; i++)
      printf(" %6.3f",target[i]);
    printf("\n");
    int icnt = 0;
    for (int i=starts[0]; i<starts[0]+subsizes[0]; i++) {
      for (int j=starts[1]; j<starts[1]+subsizes[1]; j++) {
	printf("%d,%d\n",i,j);
	ASSERTm(icnt<count,"icnt too hight");
	int isrc = i*sizes[1]+j;
	if (source[isrc]!=target[icnt])
	  printf("target location (%d,%d)->%d %e s/b %e\n",i,j,icnt,target[icnt],source[isrc]);
	icnt ++;
      }
    }
  }

  if (procno==0)
    printf("Finished\n");

  free(source); free(target);
  MPI_Finalize();
  return 0;
}
