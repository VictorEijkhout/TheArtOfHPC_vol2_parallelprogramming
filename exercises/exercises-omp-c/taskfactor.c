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
// did you know that C99 has bool/true/false?
#include <stdbool.h>
#include <math.h>
#include <omp.h>

bool try_as_factor( long int i,long int bignum ) {
  if (bignum%i==0) {
    printf("found %d\n",i);	  
    return true;
  }
  return false;
}

int main() {

  long int bignum = 179 * 181 *	191 * 193 * 197 * 199 * 211 * 223 * 227 * 229,
    //2000000111,
    maxfactor = sqrt(bignum)+1, // 42000;
  bool found = 0;

/**** your code here ****/
  for (long int i=2; i<bignum; i++) {
/**** your code here ****/
    try_as_factor(i,bignum);
    /*
     * Can you make the whole process stopp
     * when any factor has been found?
     */
/**** your code here ****/
/**** your code here ****/
  }

  return 0;
}
