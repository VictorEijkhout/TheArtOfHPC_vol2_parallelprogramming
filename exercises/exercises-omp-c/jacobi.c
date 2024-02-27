/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-6
 ****
 **** OpenMP Exercise
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc,char **argv) {

  /*
   * Set problem size
   * Use a default or set as commandline argument
   */
  long int vectorsize = 100;
  if (argc==2)
    vectorsize = atoi(argv[1]);
  printf("Using vector size=%ld\n",vectorsize);

  /*
   * Allocate vectors
   * - set solution to identically 1
   * - set rhs corresponding to Aii = 2.5, Ai,i-1 = Ai,i+1 = -1
   */
  double diag=2.01;
  double
    *solution = (double*)malloc(vectorsize*sizeof(double)),
    *rhs = (double*)malloc(vectorsize*sizeof(double)),
    *xvector = (double*)malloc(vectorsize*sizeof(double)),
    *tvector = (double*)malloc(vectorsize*sizeof(double));
  if (!solution || !rhs || !xvector || !tvector) {
    printf("Allocation failed\n"); return 1; }
  for ( long int i=0; i<vectorsize; ++i ) {
    solution[i] = 1.; rhs[i] = diag-2.; xvector[i] = i*1./vectorsize;
  }
  rhs[0] = diag-1.; rhs[vectorsize-1] = diag-1.;

  double error0,error;
  double tstart = omp_get_wtime();
  int iteration;
  for ( iteration=0; ; ++iteration ) {
    /*
     * Compute the error
     * In iteration 0, record the error
     * After that, exit if reduction by 10^5
     */
    error=0.;
    #pragma omp parallel for reduction(+:error)
    for ( long int i=0; i<vectorsize; ++i ) 
      error += pow( xvector[i]-solution[i],2 );
    error = sqrt(error);
    //printf("[%d] error=%e\n",iteration,error);
    if (iteration==0)
      error0 = error;
    else if (error<error0*1.e-3) break;
    /*
     * Compute the next iteration 
     * - conpute into a temp vector
     * - copy temp back into x
     */
    #pragma omp parallel for
    for ( long int i=0; i<vectorsize; ++i ) {
      double nxb = rhs[i];
      if (i<vectorsize-1) 
	nxb += xvector[i+1];
      if (i>0)
      	nxb +=xvector[i-1];
      tvector[i] = nxb/diag;
      // COMMENT OUT THE NEXT THREE LINES TO GET A GAUSS-JORDAN METHOD
    }
    #pragma omp parallel for
    for ( long int i=0; i<vectorsize; ++i ) {
      xvector[i] = tvector[i];
    }
    /* Print out for debugging */
    /* for ( long int i=0; i<vectorsize; ++i ) */
    /*   printf("%5.2f ",xvector[i]); printf("\n"); */
  }
  double duration = omp_get_wtime()-tstart;
  printf("Converged in %d iterations to %e reduction in time=%7.3f\n",
	 iteration,error/error0,duration);

  return 0;
}
