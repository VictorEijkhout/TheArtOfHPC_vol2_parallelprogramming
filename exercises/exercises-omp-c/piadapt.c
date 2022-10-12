/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 **** OpenMP Exercise
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define N 50000000 // 50 million

int main() {

  // step size
  double h = 1./(N+1);

  // start timer
  double t = omp_get_wtime();

  double pi,quarterpi = 0.; int nsamples=0;
  // Exercise 1:
  // -- make the for loop parallel
  // Exercise 2:
  // -- try out different loop schedules:
  //    static: this gives a baseline performance. speedup should be less than linear.
  //    static with chunksize : why may this help?
  //    dynamic: try different chunk sizes
  //    guided: does this improve over dynamic?
#pragma omp parallel
/**** your code here ****/
  for (int i=0; i<N; i++) {
    double
      x = i*h,x2 = (i+1)*h,
      y = sqrt(1-x*x),y2 = sqrt(1-x2*x2),
      slope = (y-y2)/h;
    if (slope>15) slope = 15;
    int
      samples = 1+(int)slope, is;
    for (is=0; is<samples; is++) {
      double
	hs = h/samples,
	xs = x+ is*hs,
	ys = sqrt(1-xs*xs);
      quarterpi += hs*ys;
      nsamples++;
    }
  }
  pi = 4*quarterpi;

#pragma omp parallel

// stop timer and report
  t = omp_get_wtime()-t;
  if (omp_get_thread_num()==0)
    printf("Pi=%8.4f in time on %2d threads: %7.5f\n",
	   4*quarterpi,omp_get_num_threads(),t);
  
  return 0;
}
