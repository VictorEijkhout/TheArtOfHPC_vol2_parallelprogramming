/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% Debugging example: floating point exception
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
 * Enable floating point exceptions
 */
#include <fenv.h>

/*
 * Exercise 1:
 * -- run this problem and see what output you get
 * -- run in the debugger and try to catch when the 
 *    input of the root function is negative
 */
float root(float n) 
{
  float r;
  r = sqrt(n);
  return r; 
}
 
int main() {

  /*
   * Exercise 2:
   * -- uncomment the next line and observe the exception
   *    behavior, both in the debugger and outside
   */
  //  feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);

  float x=0;
  for (int i=100; i>-100; i--)
    x += root(i+.5);
  printf("sum: %e\n",x);
  return 0;
}
