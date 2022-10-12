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
#include <omp.h>

int main() {

  int sum,how_many_threads,sumcheck;

  printf("There are %d cores available\n",omp_get_num_procs());
  
  // Exercise part 1:
  // -- fill in the routine that reports the nummber of threads.
#pragma omp parallel 
  {
    how_many_threads =
/**** your code here ****/
      ;
      printf("There are %d active threads\n",how_many_threads);
  }

  // Exercise part 2:
  // -- add the thread numbers
  // -- run the program. run it again.
  // -- run the program a number of times:
  //    for i in `seq 1 100` ; do ./parallel ; done | grep dangers
  sum=0;
#pragma omp parallel
  sum +=
/**** your code here ****/

  sumcheck = how_many_threads*(how_many_threads-1)/2;
  if (sum==sumcheck) {
    printf("You got lucky: sum is %d\n",sum);
  } else {
    printf("And here are the dangers of parallel programming: sum is %d, should be %d\n",
	   sum,sumcheck);
  }

  return 0;
}
