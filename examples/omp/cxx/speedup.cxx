/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% speedup.cxx : exploration of loop speedup
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <cmath>
#include <algorithm>

#include "omp.h"

int main(int argc,char **argv) {

#ifndef N
#define N 10000
#endif
#ifndef M
#define M N
#endif
#define STEPS 3

  vector<double> points(N);
#pragma omp parallel for
  for (int ip=0; ip<N; ip++) {
    points[ip] = rand()/(double)RAND_MAX;
  }
  vector<double> values(N);
#pragma omp parallel for
  for (int ip=0; ip<N; ip++) {
    values[ip] = 0.;
  }

  int nthreads;
#pragma omp parallel
#pragma omp single
  nthreads = omp_get_num_threads();
  printf("================ #threads = %d ================\n",nthreads);

  /*
   * Sequential version
   * mostly for reference time
   */
  double t_seq;
  {
    for (int ip=0; ip<N; ip++) {
      values[ip] = points[ip];
    }
    double tstart = omp_get_wtime();
    for (int step=0; step<STEPS; step++) {
      for (int ip=0; ip<N; ip++) {
        for (int jp=0; jp<M; jp++) {
          double f = sin( values[ip] );
          values[ip] = f;
        }
      }   
    }
    double elapsed = omp_get_wtime()-tstart; t_seq = elapsed;
    double ff=0.;
    for_each( values.begin(),values.end(),
	      [&ff] (auto v) { ff += v; }
	      );
    printf("%25s: %e; total value: %e\n","Sequential",elapsed,ff);
  }

  /*
   * Parallel 
   * indexed version
   */
  {
    for (int ip=0; ip<N; ip++) {
      values[ip] = points[ip];
    }
    double tstart = omp_get_wtime();
    for (int step=0; step<STEPS; step++) {
#pragma omp parallel for
      for (int ip=0; ip<N; ip++) {
        for (int jp=0; jp<M; jp++) {
          double f = sin( values[ip] );
          values[ip] = f;
        }
      }   
    }
    double elapsed = omp_get_wtime()-tstart;
    double ff=0.;
    for_each( values.begin(),values.end(),
	      [&ff] (auto v) { ff += v; }
	      );
    int nthreads;
#pragma omp parallel
#pragma omp single
    nthreads = omp_get_num_threads();
    printf("%25s: %e; total value: %e, speedup=%5.2f\n",
           "Parallel C style",
           elapsed,ff,t_seq/elapsed);
  }

  /*
   * Parallel
   * range-based version
   */
  {
    for (int ip=0; ip<N; ip++) {
      values[ip] = points[ip];
    }
    double tstart = omp_get_wtime();
    for (int step=0; step<STEPS; step++) {
      #pragma omp parallel for
      for ( auto& v : values ) {
        for (int jp=0; jp<M; jp++) {
          double f = sin( v );
          v = f;
        }
      }
    }
    double elapsed = omp_get_wtime()-tstart;
    double ff=0.;
    for_each( values.begin(),values.end(),
	      [&ff] (auto v) { ff += v; }
	      );
    int nthreads;
#pragma omp parallel
#pragma omp single
    nthreads = omp_get_num_threads();
    printf("%25s: %e; total value: %e, speedup=%5.2f\n",
           "Parallel range-based",
           elapsed,ff,t_seq/elapsed);
  }

  return 0;
}
