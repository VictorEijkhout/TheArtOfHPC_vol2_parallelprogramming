/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% getx.c : mucking with large integers
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

  if (procno==0) {
    printf("size of size_t = %d\n",sizeof(size_t));

    int gig = 1<<30;
    int nblocks = 8;
    size_t big1 = gig * nblocks * sizeof(double);
    size_t big2 = (size_t)1 * gig * nblocks * sizeof(double);
    size_t big3 = (size_t) gig * nblocks * sizeof(double);
    size_t big4 = gig * nblocks * (size_t) ( sizeof(double) );
    size_t big5 = sizeof(double) * gig * nblocks;
;
    printf("%lld %lld %lld %lld %lld\n",big1,big2,big3,big4,big5);
  }

  MPI_Finalize();
  return 0;
}
