/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% typesize.c : type sizing
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

  int size, count, stride, bs;

  MPI_Datatype newtype;

  count = 3; bs = 2; stride = 5;
  MPI_Type_vector(count,bs,stride,MPI_DOUBLE,&newtype);
  MPI_Type_commit(&newtype);
  MPI_Type_size(newtype,&size);
  ASSERT( size==(count*bs)*sizeof(double) );
  MPI_Type_free(&newtype);

  printf("count=%d, stride=%d, bs=%d, size=%d\n",count,stride,bs,size);

  MPI_Aint lb,asize;
  MPI_Type_vector(count,bs,stride,MPI_DOUBLE,&newtype);
  MPI_Type_commit(&newtype);
  MPI_Type_get_extent(newtype,&lb,&asize);
  ASSERT( lb==0 );
  ASSERT( asize==((count-1)*stride+bs)*sizeof(double) );
  MPI_Type_free(&newtype);

  printf("count=%d, stride=%d, bs=%d: lb=%ld, extent=%ld\n",count,stride,bs,lb,asize);

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
