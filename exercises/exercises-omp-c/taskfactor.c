/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 **** OpenMP Exercise
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main() {

  int N = 2999*3001;

  // Exercise:
  // -- turn the factor-testing block into a task
  // -- run your program several times.
  //    for i in `seq 1 1000` ; do ./taskfactor ; done | grep -v 2999
  //    why does it find the wrong solution? try to fix this
  int factor=0;
#pragma omp parallel
#pragma omp single
  for (int f=2; f<4000; f++) {
/**** your code here ****/
    { // see if `f' is a factor
      if (N%f==0) { // found factor!
/**** your code here ****/
	  factor = f;
      }
    }
    if (factor>0)
      break;
  }
  if (factor>0)
    printf("Found a factor: %d\n",factor);
  else
    printf("Prime number!\n");
  
  return 0;
}
