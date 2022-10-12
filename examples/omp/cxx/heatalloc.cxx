/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% heat.c : simple heat equation
   %%%%    exploring custom allocator for first-touch
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

#include <omp.h>

#ifndef N
#define N 10000000
#endif

template<typename T>
struct uninitialized {
  uninitialized() {};
  T val;
  constexpr operator T() const {return val;};
  double operator=( const T&& v ) { val = v; return val; };
};

int main(int argc,char **argv) {

  vector<uninitialized<double>> x(N),y(N);

#pragma omp parallel for
  for (int i=0; i<N; i++)
    y[i] = x[i] = 0.;
  x[0] = 0; x[N-1] = 1.;

  double s=0;
  double timer = omp_get_wtime();
  for (int it=0; it<1000; it++) {
#pragma omp parallel for
    for (int i=1; i<y.size()-1; i++)
      y[i] = ( x[i-1]+x[i]+x[i+1] )/3.;
#pragma omp parallel for
    for (int i=1; i<x.size()-1; i++)
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
