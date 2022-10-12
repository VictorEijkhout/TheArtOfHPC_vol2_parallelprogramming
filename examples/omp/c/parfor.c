/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% parfor.c : exploring parallel for
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>

#include "omp.h"

int main(int argc,char **argv) {

  int ncores = omp_get_num_procs();
  printf("%%%%%%%% equal thread/core counts %%%%%%%%\n");
#pragma omp parallel 
  {
    int
      nthreads = omp_get_num_threads(),
      thread_num = omp_get_thread_num();
    printf("Threads entering parallel region: %d\n",
           nthreads);
    #pragma omp for
    for (int iter=0; iter<nthreads; iter++)
      printf("thread %d executing iter %d\n",
             thread_num,iter);
  }
  
  return 0;
}
