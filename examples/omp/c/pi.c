/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% pi.c : compute pi
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

  int N=10000;
  if (argc>1) N = atoi(argv[1]);
  float h = (1./N);

  float pi4 = 0.;
  double tstart = omp_get_wtime();
#pragma omp parallel for reduction(+:pi4)
  for (int isample=0; isample<N; isample++) {
    float xsample = isample * h;
    float y = sqrt(1-xsample*xsample);
    pi4 += h*y;
  }
  double duration = omp_get_wtime()-tstart;

  const double pi=3.14159265358979323;
  double err = pi-4*pi4;
  int nt;
#pragma omp parallel
#pragma omp master
  nt = omp_get_num_threads();
  printf("Pi computed with %4.1e samples, %3d threads: err=%9.6f; time=%6.4f\n",
	 (double)N,nt,fabs(err),duration);

  return 0;
}
