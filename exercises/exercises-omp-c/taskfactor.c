/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2024
   %%%%
   %%%% taskfactor.c : use OMP tasks to factor a prime number
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

bool try_as_factor( long int i,long int bignum ) {
  if (bignum%i==0) {
    printf("found %ld\n",i);
    return true;
  }
  return false;
}

int main() {

  long int bignum = ( (long int) (179) ) * 181 * 191 * 193 * 197 * 199 * 211 * 223,
    maxfactor = sqrt(bignum)+1;
  bool found = 0;

#pragma omp parallel
#pragma omp single
  {
    for (long int f=2; f<maxfactor; f++) {
#pragma omp task
      {
	bool found_ = try_as_factor(f,bignum);
#pragma omp atomic
	found += found_;
      }
      /*
       * Can you make the whole process stopp
       * when any factor has been found?
       */
      if (found) break;
    }
  }

  return 0;
}
