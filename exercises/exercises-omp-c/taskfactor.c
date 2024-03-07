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
#include <math.h>
#include <omp.h>

int bignum = 2000000111, maxfactor = 42000, the_factor = -1;

void try_as_factor( int i ) {
  /*
   * Can you make the whole process stopp
   * when any factor has been found?
   */
/**** your code here ****/

  if (bignum%i==0) {
    {
      printf("found %d\n",i);	  
      the_factor = i;
    }
  }
  return;
}

int main() {

/**** your code here ****/
  for (int i=2; i<bignum; i++) {
/**** your code here ****/
    try_as_factor(i);
  }
/**** your code here ****/

  return 0;
}
