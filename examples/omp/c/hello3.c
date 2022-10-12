/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% hello3.c : different ways of thread counting
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>

#include "omp.h"

int main(int argc,char **argv) {

  int mythread,nthreads;
  nthreads = omp_get_num_threads();
  mythread = omp_get_thread_num();
  printf("Hello from %d out of %d outside the region\n",mythread,nthreads);

#pragma omp parallel
  {
    int mythread,nthreads;
    nthreads = omp_get_num_threads();
    mythread = omp_get_thread_num();
    printf("Hello from %d out of %d inside the region\n",mythread,nthreads);
  }

#pragma omp parallel
  {
    nthreads = omp_get_num_threads();
    mythread = omp_get_thread_num();
    printf("Hello from %d out of %d inside the nonprivate region\n",mythread,nthreads);
  }

  return 0;
}
