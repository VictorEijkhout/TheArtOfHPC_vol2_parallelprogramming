/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2024
   %%%%
   %%%% histogramlock : histogramming with locks
   %%%%
   %%%% Usage: histprogram [ nbins ]
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

int main(int argc,char **argv)
{

  /*
   * Problem parameters
   */
  int nbins = 10;
  if (argc>1)
    nbins = atoi(argv[1]);
  long int nexperiments = 10000000;

  /*
   * Create histogram bins
   */
  long int bins[nbins];
  for (int ibin=0; ibin<nbins; ++ibin)
    bins[ibin] = 0;
  /*
   * Create a lock for each bin
   */
  omp_lock_t locks[nbins];
/**** your code here ****/

  int nthreads;
#pragma omp parallel
#pragma omp master
  nthreads = omp_get_num_threads();
  
  /*
   * Timed experiment loop
   */
  double tstart = omp_get_wtime();
#pragma omp parallel
  {
    int seed = omp_get_thread_num();
    for ( long int experiment=0; experiment<nexperiments; ++experiment ) {
      int bin;
      for ( int r=0; r<100; r++ ) bin = rand_r(&seed)%nbins;
      /*
       * Lock the bin
       */
/**** your code here ****/
      bins[bin]++;
      /*
       * and ?
       */
/**** your code here ****/
    }
  }
  double duration = omp_get_wtime()-tstart;
  
  /*
   * Sanity check
   */
  long int sum = 0;
  for (int ibin=0; ibin<nbins; ++ibin)
    sum += bins[ibin];

  /*
   * Result print
   */
  printf("Summing up to %ld s/b %ld\n",sum,nthreads*nexperiments);
  printf(" .. runtime = %7.3f\n",duration);

  /*
   * Neatly destroy locks again
   */
  for (int ibin=0; ibin<nbins; ++ibin)
    omp_destroy_lock( locks+ibin );

  return 0;
}
