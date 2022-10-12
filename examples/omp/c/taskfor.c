/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% hello.c : simple hello world
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <omp.h>

int main(int argc,char **argv) {

#define N 10000000
  int ncores = omp_get_num_procs();
  double tsequential=0;
  for ( int nthreads=1; nthreads<=ncores; nthreads+=2 ) {
    float *x = malloc(N*sizeof(float)),
      *y = malloc(N*sizeof(float));;

#pragma omp parallel for num_threads(nthreads)
    for (int i=0; i<N; i++) {
      x[i] = 1.1*i+.5; y[i] = 0.;
    }

    double tstart = omp_get_wtime();
#pragma omp parallel num_threads(nthreads)
#pragma omp single
    {
      int np = omp_get_num_threads();
      size_t block = (N+np-1)/np;
      for ( int t=0; t<np; t++ ) {
        int first = t*block,
          last = t==np-1 ? N : (t+1)*block;
#       pragma omp task shared(x,y)
        {
          for ( int i=first; i<last; i++ ) {
            y[i] = 2.2*x[i];
          }
        } // end task
      } // end for
#     pragma omp taskwait
    } // end single
    double msec = 1000*( omp_get_wtime()-tstart );
    if (nthreads==1) tsequential = msec;
    printf("#threads=%d, time=%7.4fmsec",nthreads,msec);
    if (nthreads>1) printf(", speedup=%7.4f",tsequential/msec);
    printf("\n");

#define APPROX(x,y)                                        \
    ( (x)>(y)*(1.-1.e-5) && (x)<(y)*(1.+1.e-5) )
    int correct=1;
    for (int i=0; i<N; i++) {
      float is = y[i],sb = 2.2*x[i];
      if ( ! APPROX( is,sb ) ) { correct = 0;
        printf("%5d: %7.4f != %7.4f\n",i,is,sb);
      }
    }
    if (correct) printf("all results correct\n");
    free(x); free(y);
  }

  return 0;
}
