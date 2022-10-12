/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% ireductinit.c : behaviour of initializer in user-defined reduction
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

int imax(int r,int n) {
  // r is the already reduced value
  // n is the new value
  int m;
  if (n>r) {
    m = n;
  } else {
    m = r;
  }
  m++;
  printf("combine %d %d : %d\n",r,n,m);
  return m;
}

int main(int argc,char **argv) {

  int m,ndata = 4, data[4] = {2,-3,0,5};

#pragma omp declare reduction \
  (rwz:int:omp_out=imax(omp_out,omp_in)) \
  initializer(omp_priv=INT_MIN)

  m = 7;
#pragma omp parallel for reduction(rwz:m)
  for (int idata=0; idata<ndata; idata++)
    m = imax(m,data[idata]);

  if (m!=5)
    printf("Parallel: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Finished\n");
  
  return 0;
}
