/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-8
   %%%%
   %%%% sharing.c : the effects of binding on writing to shared
   %%%%             original program by Richard Todd Evans
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdio.h>
#include <omp.h>


int main() {

  int i,j;
  int reps = 1000;
  int N = 8*10000;

  double start, stop, delta;
  double a;

#pragma omp parallel
  a = 0;

  start = omp_get_wtime();  
#pragma omp parallel
  { // not a parallel for: just a bunch of reps
    for (int j = 0; j < reps; j++) {
#pragma omp for schedule(static,1)
      for (int i = 0; i < N; i++){
#pragma omp atomic
	a++;  
      }
      
    }
  }
  stop  = omp_get_wtime();
  delta = ((double)(stop - start))/reps;
  printf("run time = %fusec\n", 1.0e6*delta);

  printf("sum = %.1f\n", a);
  
  return 0;
}
