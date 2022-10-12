/****************************************************************
 ****************************************************************
 ****
 ****
 **** This program file is part of the book and course
 **** "Parallel Computing"
 **** by Victor Eijkhout, copyright 2013-6
 ****
 **** allgatherv.c : vector allgather, task t contributes t+1 data
 ****
 ****************************************************************
 ****************************************************************
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  
#include "globalinit.c"
  
  int my_count = procno+1;
  int *my_array = (int*) malloc(my_count*sizeof(int));
  for (int i=0; i<my_count; i++)
    my_array[i] = procno;
  int *recv_counts = (int*) malloc(nprocs*sizeof(int));
  int *recv_displs = (int*) malloc(nprocs*sizeof(int));

  MPI_Allgather
    ( &my_count,1,MPI_INT,
      recv_counts,1,MPI_INT, comm );
  int accumulate = 0;
  for (int i=0; i<nprocs; i++) {
    recv_displs[i] = accumulate; accumulate += recv_counts[i]; }
  int *global_array = (int*) malloc(accumulate*sizeof(int));
  MPI_Allgatherv
    ( my_array,procno+1,MPI_INT,
      global_array,recv_counts,recv_displs,MPI_INT, comm );
  
  if (procno==0) {
    for (int p=0; p<nprocs; p++)
      if (recv_counts[p]!=p+1)
	printf("count[%d] should be %d, not %d\n",
	       p,p+1,recv_counts[p]);
    int c = 0;
    for (int p=0; p<nprocs; p++)
      for (int q=0; q<=p; q++)
	if (global_array[c++]!=p)
	  printf("p=%d, q=%d should be %d, not %d\n",
		 p,q,p,global_array[c-1]);
  }

  MPI_Finalize();
  return 0;
}
