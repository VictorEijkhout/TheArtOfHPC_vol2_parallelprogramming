/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-8
   %%%%
   %%%% reductpositive.c : user-defined reduction
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

//int reduce_without_zero(int r,int n);
void reduce_without_zero(void *in,void *inout,int *len,MPI_Datatype *type) {
  // r is the already reduced value, n is the new value
  int n = *(int*)in, r = *(int*)inout;
  int m;
  if (n==0) { // new value is zero: keep r
    m = r;
  } else if (r==0) {
    m = n;
  } else if (n<r) { // new value is less but not zero: use n
    m = n;
  } else { // new value is more: use r
    m = r;
  };
#ifdef DEBUG
  printf("combine %d %d : %d\n",r,n,m);
#endif
  // return the new value
  *(int*)inout = m;  
}

int main(int argc,char **argv) {

#include "globalinit.c"

  int m,mreduct=2000000000,ndata = 10, data[10] = {2,3,0,5,0,1,8,12,4,0},
    positive_minimum;
  if (nprocs>ndata) {
    printf("Too many procs for this example: at most %d\n",ndata);
    return 1;
  }
    
  for (int i=0; i<nprocs; i++)
    if (data[i]<mreduct && data[i]>0)
      mreduct = data[i];

  MPI_Op rwz;
  MPI_Op_create(reduce_without_zero,1,&rwz);
  MPI_Allreduce(data+procno,&positive_minimum,1,MPI_INT,rwz,comm);

  // check that the distributed result is the same as sequential
  if (mreduct!=positive_minimum)
    printf("[%d] Result %d should be %d\n",
           procno,positive_minimum,mreduct);
  else if (procno==0)
    printf("User-defined reduction successful: %d\n",positive_minimum);

  MPI_Finalize();
  return 0;
}
