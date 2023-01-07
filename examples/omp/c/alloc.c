/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% alloc.c : dealing with allocated data
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int argc,char **argv) {

  {
    int nthreads;
#pragma omp parallel
#pragma omp master
    nthreads = omp_get_num_threads();

    int *array =
      (int*) malloc(nthreads*sizeof(int));
    for (int i=0; i<nthreads; i++)
      array[i] = 0;

    #pragma omp parallel firstprivate(array)
    {
      int t = omp_get_thread_num();
      array += t;
      array[0] = t;
    }

    printf("Pointer\n");
    printf("Array result:\n");
    for (int i=0; i<nthreads; i++)
      printf("%d:%d, ",i,array[i]);
    printf("\n");
    printf(".. pointer\n");
  }

  {
    int nthreads;
#pragma omp parallel
#pragma omp master
    nthreads = omp_get_num_threads();

    int array[nthreads];
    for (int i=0; i<nthreads; i++)
      array[i] = 0;

#pragma omp parallel firstprivate(array)
    {
      int t = omp_get_thread_num();
      array[t] = t+1;
    }

    printf("Private\n");
    printf("Array result:\n");
    for (int i=0; i<nthreads; i++)
      printf("%d:%d, ",i,array[i]);
    printf("\n");
    printf(".. private\n");
  }

#if 0
  I HAVE NO IDEA WHAT THIS DOES
  {
    int nthreads=4;

    int array[nthreads];
    for (int i=0; i<nthreads; i++)
      array[i] = 0;

    {
      int t = 2;
      array += t;
      array[0] = t;
    }

    printf("Array result:\n");
    for (int i=0; i<nthreads; i++)
      printf("%d:%d, ",i,array[i]);
    printf("\n");
  }
#endif

  return 0;
}
