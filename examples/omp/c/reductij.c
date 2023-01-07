/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-8
   %%%%
   %%%% reductij.c : user-defined reduction on manhattan distance. 
   %%%% NON-FUNCTIONAL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

typedef struct { double i; double j; } coordinate;

double manhattan_dist( coordinate *r,coordinate *n ) {
  // r is the already reduced value
  // n is the new value
  int m;
  if (*r==-1){ // initializer
    if (n!=0) {
      m = n;
    } else {
      m = r;
    }
  } else if (n==0) { // new value is zero: keep r
    m = r;
  } else if (n<r) { // new value is less but not zero: use n
    m = n;
  } else { // new value is more: use r
    m = r;
  };
  printf("combine %d %d : %d\n",r,n,m);
  return m;
}

int main(int argc,char **argv) {

  int m,ndata = 4, data[4] = {2,3,0,5};

#pragma omp declare reduction \
  (rwz:int:omp_out=reduce_without_zero(omp_out,omp_in)) \
  initializer(omp_priv=-1)

  m = -1;
  for (int idata=0; idata<ndata; idata++)
    m = reduce_without_zero(m,data[idata]);
  if (m!=2)
    printf("Sequential: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Sequential case succeeded\n");
  
  m = -1;
#pragma omp parallel for reduction(rwz:m)
  for (int idata=0; idata<ndata; idata++)
    m = reduce_without_zero(m,data[idata]);

  if (m!=2)
    printf("Parallel: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Finished\n");
  
  return 0;
}
