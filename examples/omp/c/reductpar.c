/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% reductpar.c : reduction without loop
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

int main(int argc,char **argv) {

  int m,ndata = 4, data[4] = {2,-3,0,5};

  m = INT_MIN;
  for (int idata=0; idata<ndata; idata++) {
    int d = data[idata];
    m = d>m ? d : m;
  }
  if (m!=5)
    printf("Sequential: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Sequential case succeeded\n");
  
  m = INT_MIN;
#pragma omp parallel reduction(max:m) num_threads(ndata)
  {
    int t = omp_get_thread_num();
    int d = data[t];
    m = d>m ? d : m;
  };

  if (m!=5)
    printf("Parallel: wrong reduced value: %d, s/b %d\n",m,2);
  else
    printf("Finished with correct parallel result\n");
  
  return 0;
}
