/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% ireduct.c : recreate integer max as user-defined reduction
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

//snippet ompmymax
int mymax(int r,int n) {
  // r is the already reduced value
  // n is the new value
  int m;
  if (n>r) {
    m = n;
  } else {
    m = r;
  }
  //snippet end
  printf("combine %d %d : %d\n",r,n,m);
  //snippet ompmymax
  return m;
}
//snippet end

int main(int argc,char **argv) {

  int m,ndata = 4, data[4] = {2,-3,0,5};

  //snippet ompmymax
#pragma omp declare reduction \
  (rwz:int:omp_out=mymax(omp_out,omp_in)) \
  initializer(omp_priv=INT_MIN)
  //snippet end
  
  m = INT_MIN;
  for (int idata=0; idata<ndata; idata++)
    m = mymax(m,data[idata]);
  if (m!=5)
    printf("Sequential: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Sequential case succeeded\n");
  
  //snippet ompmymax
  m = INT_MIN;
#pragma omp parallel for reduction(rwz:m)
  for (int idata=0; idata<ndata; idata++)
    m = mymax(m,data[idata]);
  //snippet end

  if (m!=5)
    printf("Parallel: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Finished\n");
  
  return 0;
}
