/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% taskreduct : OpenMP reduction on task results
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main() {

  int bound=100, sum=0;
  
#pragma omp parallel
#pragma omp single
  {
  #pragma omp taskgroup task_reduction(+:sum)
  for (int itask=1; itask<=bound; itask++) {
    #pragma omp task in_reduction(+:sum)
    sum += itask;
    }
  }

  printf("Final result=%d, s/b: %d\n",sum,bound*(bound+1)/2);

  return 0;
}
