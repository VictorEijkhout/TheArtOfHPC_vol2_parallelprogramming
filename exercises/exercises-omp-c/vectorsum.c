/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2024
   %%%%
   %%%% vectorsum.c : parallel loops
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main( int argc,char **argv ) {

  /*
   * Read the vector size from the commandline
   * or use a default
   */
  int vectorsize=10;
  if (argc==2)
    vectorsize = atoi(argv[1]);
  printf("Using vectorsize: %d\n",vectorsize);

  /*
   * Get the number of threads
   * This is not really used anywhere
   */
  int nthreads;
#pragma omp parallel
#pragma omp master
  nthreads = omp_get_num_threads();

  /*
   * Allocate input and output array
   * and initialize
   */
  double 
    *invec = (double*) malloc(vectorsize*sizeof(double)),
    *outvec = (double*) malloc(vectorsize*sizeof(double));
  if (!invec || !outvec) {
    printf("Could not allocate\n"); return 1; }
  for (int i=0; i<vectorsize; i++) {
    invec[i] = rand()/(double)RAND_MAX;
    outvec[i] = 0;
  }

  /*
   * Set up loop coefficients:
   * each coefficient will be used throughout
   * an inner loop
   */
  const int nloops=500;
  double *loopcoeff = (double*) malloc( nloops*sizeof(double) );
  loopcoeff[0] = 1.;
  for ( int iloop=1; iloop<nloops; ++iloop )
    loopcoeff[iloop] = loopcoeff[iloop-1]*1.1;

  /*
   * This is the sequential version
   * - copy this block and make a parallel version
   * - does the sequential version run at the same
   *   speed as the single-trhreaded parallel?
   */
  {
    double tstart = omp_get_wtime();
    double factor=1;
    for ( int iloop=0; iloop<nloops; ++iloop ) {
      for ( int i=0; i<vectorsize; ++i ) {
        outvec[i] += invec[i]*loopcoeff[iloop];
      }
    }
    double duration = omp_get_wtime()-tstart;
    printf("Sequential t= %8.5f sec\n",duration);
  }

  {
    double tstart = omp_get_wtime();
/**** your code here ****/
    double duration = omp_get_wtime()-tstart;
    printf("Threads %2d t= %8.5f sec\n",nthreads,duration);
  }

  /*
   * In case the compiler is really clever,
   * here we prevent optimizing by elimination
   */
  double s=0.;
  for (int i=0; i<vectorsize; i++) {
    s += outvec[i];
  }
  if (s<0) printf("%e\n",s);
  
  free(invec); free(outvec); free(loopcoeff);

  return 0;
}
