/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% Debugging example
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>

#ifndef N
#define N 100
#endif

int main(int argc,char **argv) {

  float *squares;
  squares = (float*) malloc(N*sizeof(float));

  /*
   * Exercise 2:
   * -- remove the initialization
   * -- run the code in valgrind and try 
   *    to understand the output
   */
  float sum=0.;


  /*
   * This loop uses 1-based indexing
   * in a 0-based array
   */
  for (int i=1; i<=N; i++) {
    /*
     * Exercise:
     * -- memory violation on the last iteration.
     *    try to discover the value of 'i' for which the 
     *    violation occurs.
     */
    squares[i] = 1./(i*i);
    sum += squares[i];
  }    
  printf("Sum: %e\n",sum);

  free(squares);

  return 0;
}
