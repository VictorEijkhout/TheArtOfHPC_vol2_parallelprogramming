/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% scan.c : scanning
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <omp.h>

int main(int argc,char **argv) {

  int nthreads;
#pragma omp parallel
#pragma omp single
  nthreads = omp_get_num_threads();

  int length = 2*nthreads;
  int *amounts = (int*)malloc( length*sizeof(int) );
  for (int i=0; i<length; i++)
    amounts[i] = i;
  printf("Amounts: ");
  for (int i=0; i<length; i++)
    printf(" %2d",amounts[i]);
  printf("\n");

  int *partials = (int*)malloc( length*sizeof(int) );
  int partial_sum;

  printf("Inclusive scan\n");
  for (int i=0; i<length; i++)
    partials[i] = 0;
  partial_sum = 0;
#pragma omp parallel for reduction(inscan,+:partial_sum)
  for (int i=0; i<length; i++) {
    partial_sum += amounts[i];
#   pragma omp scan inclusive(partial_sum)
    partials[i] = partial_sum;
  }
  printf("Partials:");
  for (int i=0; i<length; i++ )
    printf(" %2d",partials[i]);
  printf("\n");
  printf("reduction result: %d\n",partial_sum);
  
#if 0
  printf("Exclusive scan\n");
  for (int i=0; i<length; i++)
    partials[i] = 0;
  partial_sum = 0;
#pragma omp parallel for reduction(inscan,+:partial_sum)
  for (int i=0; i<length; i++) {
    partial_sum += amounts[i];
#   pragma omp scan exclusive(partial_sum)
    partials[i] = partial_sum;
  }
  printf("Partials:");
  for (int i=0; i<length; i++ )
    printf(" %d",partials[i]);
  printf("\n");
  printf("reduction result: %d\n",partial_sum);
#endif

  return 0;
}
