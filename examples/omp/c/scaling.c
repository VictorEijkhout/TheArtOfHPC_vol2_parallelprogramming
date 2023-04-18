/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% speedup.c : exploration of loop speedup
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "omp.h"

int main(int argc,char **argv) {

#ifndef N
#define N 10000
#endif
#ifndef M
#define M N
#endif
#define STEPS 3

  double *points = (double*) malloc( N * sizeof(double) );
#pragma omp parallel for
  for (int ip=0; ip<N; ip++) {
    points[ip] = rand()/(double)RAND_MAX;
  }
  double *values = (double*) malloc( N * sizeof(double) );
#pragma omp parallel for
  for (int ip=0; ip<N; ip++) {
    values[ip] = 0.;
  }

  int nthreads,nprocs;
#pragma omp parallel
#pragma omp single
  {
    nthreads = omp_get_num_threads();
    nprocs = omp_get_num_procs();
  }
  printf("================ #threads = %d ================\n",nthreads);
  if (nthreads>nprocs)
    printf("WARNING: nthreads > nprocs=%d\n",nprocs);

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
      //codesnippet scalingsineloop
      for (int ip=0; ip<N; ip++) {
        for (int jp=0; jp<M; jp++) {
          double f = sin( values[jp] );
          values[ip] += f;
        }
	if (ip>0)
          values[ip] += values[ip-1];
      }   
      //codesnippet end
    }
    double elapsed = omp_get_wtime()-tstart; t_seq = elapsed;
    double ff=0.;
    for (int ip=0; ip<N; ip++) {
      ff += values[ip];
    }
    printf("%25s: %e; total value: %e\n","Sequential",elapsed,ff);
  }

  /*
   * Parallel version
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
          double f = sin( values[jp] );
          values[ip] += f;
        }
	if (ip>0)
          values[ip] += values[ip-1];
      }   
    }
    double elapsed = omp_get_wtime()-tstart;
    double ff=0.;
    for (int ip=0; ip<N; ip++) {
      ff += values[ip];
    }
    int nthreads;
#pragma omp parallel
#pragma omp single
    nthreads = omp_get_num_threads();
    printf("%25s: %e;  checksum: %e, speedup= %5.2f\n",
           "Parallel",
           elapsed,ff,t_seq/elapsed);
  }

  free(points); free(values);
  return 0;
}
