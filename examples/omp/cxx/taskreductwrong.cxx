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

#define max(a,b) (a)>(b) ? (a) : (b)

int main() {

  int bound=10000;
  int result = bound*(bound+1)/2;

  int correct=0,wrong=0,aberr=0;
  for ( int i=0; i<100; i++) {
    int sum=0;
  
#pragma omp parallel
#pragma omp single
    {
      for (int itask=1; itask<=bound; itask++) {
	auto addin = [&sum] (int contrib) { sum += contrib; };
        #pragma omp task firstprivate(sum)
	addin(itask);
      }
      #pragma omp taskwait
    }
    if (sum==result) correct++;
    else {
	wrong++;
	aberr = max(aberr,abs(sum-result));
    }
  }
  printf("Lambda reduction correct=%d, wrong=%d, max by %d out of %d\n",
	  correct,wrong,aberr,result);

  return 0;
}
