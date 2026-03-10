/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2026
   %%%%
   %%%% nestedloops.c : nested loops
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define N 1000
#define M 500

int main( int argc,char **argv ) {

  /*
   * Read the vector size from the commandline
   * or use a default
   */
  int vectorsize=10;
  if (argc==2)
    vectorsize = atoi(argv[1]);
  printf("Using vectorsize: %d\n",vectorsize);

  int nloops=100;
  double
    *a = (double*) malloc( nloops*vectorsize*sizeof(double) )
    ,
    *b = (double*) malloc( nloops*vectorsize*sizeof(double) )
    ;
#define INDEX(i,j) (i)*vectorsize + (j)

  /*
   * Do you initialize the loops?
   * You may get some interesting behavior differences
   */
  // MISSING

  for (int i = 1; i < nloops; i++) {
    for (int j = 0; j < vectorsize; j++) {
      a[INDEX(i,j)] = 1.; b[INDEX(i,j)] = 1.;
    }
  }
  /*
  {
    /*
     * Sequential version
     */
    double tstart = omp_get_wtime();
    for (int i = 1; i < nloops; i++) {
      for (int j = 0; j < vectorsize; j++) {
        a[INDEX(i,j)] = a[INDEX(i-1,j)] + b[INDEX(i,j)];
      }
    }
    double duration = omp_get_wtime()-tstart;
    printf("Sequential t= %8.5f sec\n",duration);
  }

  {
    /*
     * OpenMP version
     */
    double tstart = omp_get_wtime();
    // MISSING
    double duration = omp_get_wtime()-tstart;
    printf("OpenMP     t= %8.5f sec\n",duration);
  }

  printf( "%e\n",a[INDEX(nloops-1,vectorsize-1)] );

  return 0;
}
