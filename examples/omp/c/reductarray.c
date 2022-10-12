/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% reductarray.c : reduction on array
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

int main(int argc,char **argv) {

  int nthreads;
#pragma omp parallel
#pragma omp single
  nthreads = omp_get_num_threads();

  {
    int data[nthreads];
    for (int i=0; i<nthreads; i++)
      data[i] = 0;

#pragma omp parallel for schedule(static,1) reduction(+:data[:nthreads])
    for (int it=0; it<nthreads; it++) {
      for (int i=0; i<nthreads; i++)
	data[i]++;
    }
    printf("Data should be ===%d:",nthreads);
    for (int i=0; i<nthreads; i++)
      printf(" %d",data[i]);
    printf("\n");
  }

  {
    int *alloced = (int*)malloc( nthreads*sizeof(int) );
    for (int i=0; i<nthreads; i++)
      alloced[i] = 0;
#pragma omp parallel for schedule(static,1) reduction(+:alloced[:nthreads])
    for (int it=0; it<nthreads; it++) {
      for (int i=0; i<nthreads; i++)
	alloced[i]++;
    }
    printf("Data should be ===%d:",nthreads);
    for (int i=0; i<nthreads; i++)
      printf(" %d",alloced[i]);
    printf("\n");
  }
  
  return 0;
}
