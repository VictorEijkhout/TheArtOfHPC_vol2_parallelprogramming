/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% heat.cxx : simple heat equation
   %%%%    array access through unique pointer
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <memory>
#include <vector>
#include <cstdio>
using namespace std;

#include <omp.h>

#ifndef N
#define N 10000000
#endif

int main(int argc,char **argv) {

  // C++20 syntax:
  // auto x = make_unique_for_overwrite(N);
  // auto y = make_unique_for_overwrite(N);

  unique_ptr<double[]> x( new double[N] );
  unique_ptr<double[]> y( new double[N] );

#pragma omp parallel for
  for (int i=0; i<N; i++) {
    y[i] = x[i] = 0.;
  }
  x[0] = 0; x[N-1] = 1.;

  double s=0;
  double timer = omp_get_wtime();
  for (int it=0; it<1000; it++) {
#pragma omp parallel for
    for (int i=1; i<N-1; i++)
      y[i] = ( x[i-1]+x[i]+x[i+1] )/3.;
#pragma omp parallel for
    for (int i=1; i<N-1; i++)
      x[i] = y[i];
  }
#pragma omp parallel for reduction(+:s)
  for (int i=0; i<N; i++)
    s += y[i];
  timer = omp_get_wtime()-timer;

#pragma omp parallel
#pragma omp single
  printf("Time=%8.4f #ops=%9.3e on %d threads (checksum %e)\n",
	 timer,(double)(2*100*N),omp_get_num_threads(),s);

  return 0;
}
