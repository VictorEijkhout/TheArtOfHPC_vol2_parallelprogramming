/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2025
   %%%%
   %%%% reductpositive.c : user-defined reduction
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

//codesnippet omprwzsig
int reduce_without_zero(int r,int n);
//codesnippet end

int main(int argc,char **argv) {

  int m,mreduct=2000000000,ndata = 10, data[10] = {2,3,0,5,0,1,8,12,4,0};

  for (int i=0; i<ndata; i++)
    if (data[i]<mreduct && data[i]>0)
      mreduct = data[i];

  //codesnippet omprwz
#pragma omp declare reduction \
  (rwz:int:omp_out=reduce_without_zero(omp_out,omp_in)) \
  initializer(omp_priv=-1)
  //codesnippet end

  m = -2;
  for (int idata=0; idata<ndata; idata++)
    m = reduce_without_zero(m,data[idata]);
  if (m!=mreduct)
    printf("Sequential: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Sequential case succeeded\n");
  
  //codesnippet omprwz
  m = -1;
#pragma omp parallel for reduction(rwz:m)
  for (int idata=0; idata<ndata; idata++)
    m = reduce_without_zero(m,data[idata]);
  //codesnippet end

  if (m!=mreduct)
    printf("Parallel: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Finished: parallel reduction was correct\n");
  
  return 0;
}

//codesnippet omprwzfun
int reduce_without_zero(int r,int n) {
  // r is the already reduced value, n is the new value
  int m;
  if (r<0){ // initializer
    if (n!=0)
      m = n;
    else
      m = r;
  } else if (n<0) { // initializer
    if (r!=0)
      m = r;
    else
      m = n;
  } else if (n==0) { // new value is zero: keep r
    m = r;
  } else if (n<r) { // new value is less but not zero: use n
    m = n;
  } else { // new value is more: use r
    m = r;
  };
  //codesnippet end
#ifdef DEBUG
  printf("combine %d %d : %d\n",r,n,m);
#endif
  //codesnippet omprwzfun
  return m;
}
//codesnippet end

