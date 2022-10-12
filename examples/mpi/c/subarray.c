/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% subarray.c : subarray datatype
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
    count = 4;
  double *source,*target;
  int sizes[2] = {4,4},subsizes[2] = {2,2},starts[2] = {1,1};
  source = (double*) malloc(sizes[0]*sizes[1]*sizeof(double));
  target = (double*) malloc(subsizes[0]*subsizes[1]*sizeof(double));

  for (int i=0; i<sizes[0]*sizes[1]; i++)
    source[i] = i+.5;

  MPI_Datatype subarraytype;
  if (procno==sender) {
    MPI_Type_create_subarray
      (2,sizes,subsizes,starts,MPI_ORDER_C,MPI_DOUBLE,&subarraytype);
    MPI_Type_commit(&subarraytype);
    MPI_Send(source,1,subarraytype,the_other,0,comm);
  } else if (procno==receiver) {
    MPI_Status recv_status;
    int recv_count;
    MPI_Recv(target,count,MPI_DOUBLE,the_other,0,comm,
      &recv_status);
    MPI_Get_count(&recv_status,MPI_DOUBLE,&recv_count);
    ASSERT(recv_count==count);
  }
  
  if (procno==sender) {
    MPI_Aint true_lb,true_extent;
    MPI_Type_get_true_extent
      (subarraytype,&true_lb,&true_extent);
    printf("Found lb=%ld, extent=%ld\n",true_lb,true_extent);
    MPI_Aint
      comp_lb = ( starts[0]*sizes[1]+starts[1] )*sizeof(double),
      comp_extent = ( (starts[0]+subsizes[0]-1)*sizes[1] + starts[1]+subsizes[1] )*sizeof(double) - comp_lb;
    printf("Computing lb=%ld extent=%ld\n",comp_lb,comp_extent);
    ASSERTm(true_lb==comp_lb,"true lb wrong");
    ASSERTm(true_extent==comp_extent,"true extent wrong");
    MPI_Type_free(&subarraytype);
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

  /* MPI_Finalize(); */
  return 0;
}
