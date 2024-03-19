/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2024
   %%%%
   %%%% histogramlock : histogramming with locks
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
   * Allocate and initialize a lock
   */
  //codesnippet lockinit
  omp_lock_t the_lock;
  omp_init_lock( &the_lock );
  //codesnippet end

  int nthreads;
#pragma omp parallel
#pragma omp master
  nthreads = omp_get_num_threads();
  
  int sum=0;
  /*
   * Each thread 
   * - sets the lock
   * - updates shared variable
   * - unsets
   */
  //codesnippet lockupdate
#pragma omp parallel
  {
    omp_set_lock( &the_lock );
    sum += omp_get_thread_num();
    omp_unset_lock( &the_lock );
  }
  //codesnippet end

  /*
   * Sanity check
   */
  printf("Summing up to %d s/b %d\n",sum,nthreads*(nthreads-1)/2);

  /*
   * Deallocate lock
   */
  //codesnippet lockinit
  omp_destroy_lock( &the_lock );
  //codesnippet end

  return 0;
}
