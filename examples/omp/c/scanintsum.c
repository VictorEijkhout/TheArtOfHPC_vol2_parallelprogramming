/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% scan1.c : sum of squares through scan
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

void print_array( int ar[],int n,char *label) {
  printf("%s:",label);
  for (int i=0; i<n; i++)
    printf(" %2d",ar[i]);
  printf("\n");
}

int main(int argc,char **argv) {

  int nthreads;
#pragma omp parallel
#pragma omp single
  nthreads = omp_get_num_threads();
  
  int *amounts = (int*)malloc( nthreads*sizeof(int) );
  for (int i=0; i<nthreads; i++)
    amounts[i] = i+1;
  print_array( amounts,nthreads,"Summing  " );
  int *inc_partials = (int*)malloc( nthreads*sizeof(int) );
  for (int i=0; i<nthreads; i++)
    inc_partials[i] = 0;
  int *exc_partials = (int*)malloc( nthreads*sizeof(int) );
  for (int i=0; i<nthreads; i++)
    exc_partials[i] = 0;
  

  int partial_sum;
  //codesnippet scanincexc
  partial_sum=0;
#pragma omp parallel for \
  reduction(inscan,+:partial_sum)
  for (int i=0; i<nthreads; i++) {
    partial_sum += amounts[i];
#   pragma omp scan inclusive(partial_sum)
    inc_partials[i] = partial_sum;
  }
  //codesnippet end
  print_array( inc_partials,nthreads,"Inclusive" );

  //codesnippet scanincexc
  partial_sum=0;
#pragma omp parallel for \
  reduction(inscan,+:partial_sum)
  for (int i=0; i<nthreads; i++) {
    exc_partials[i] = partial_sum;
#   pragma omp scan exclusive(partial_sum)
    partial_sum += amounts[i];
  }
  //codesnippet end
  print_array( exc_partials,nthreads,"Exclusive" );

  return 0;
}
