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

#ifndef N
#define N 100
#endif

#include <stdlib.h>
#include <stdio.h>
int main(int argc,char **argv) {

  float *squares,sum;

  squares = (float*) malloc(N*sizeof(float));
  /*
   * Let's see how far out of bounds we can go
   * before it triggers a memory error.
   */
  for (int i=1; ; i++) {
    /*
     * memory violation on the last iteration.
     * read the value of 'i' for which the 
     * violation occurs
     */
    squares[i] = 1./(i*i);
    sum += squares[i];
  }    
  printf("Sum: %e\n",sum);

  return 0;
}
