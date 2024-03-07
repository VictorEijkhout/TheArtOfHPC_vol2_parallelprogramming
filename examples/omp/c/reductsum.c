/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2024
   %%%%
   %%%% reductsum.c : simple inner product sum
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

int main(int argc,char **argv) {

  int ndata = 1000;
  float
    *x = (float*)malloc( ndata*sizeof(float) ),
    *y = (float*)malloc( ndata*sizeof(float) );

  //codesnippet inprodsum
  float s = 0;
  #pragma omp parallel for reduction(+:s)
  for (int idata=0; idata<ndata; idata++)
    s += x[idata]*y[idata];
  //codesnippet end

  return 0;
}
