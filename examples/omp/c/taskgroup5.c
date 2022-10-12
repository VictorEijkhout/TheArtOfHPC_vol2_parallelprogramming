/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% taskgroup.c : using the taskgroup directive
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>

int main(int argc,char **argv) {
  int N = 1000;
  if (argc>1) {
    if (!strcmp(argv[1],"-h")) {
      printf("usage: %s [nnn]\n",argv[0]);
      return 0;
    }
    N = atoi(argv[1]);
    if (N>99) {
      printf("Sorry, this overflows: setting N=99\n");
      N = 99;
    }
  }

  long int *fibo_values = (long int*)malloc(N*sizeof(long int));

  fibo_values[0] = 1; fibo_values[1] = 1;
#pragma omp parallel
#pragma omp single
  {
    for (int i=2; i<N; i++) {
      long int fs[2];
#pragma omp task shared(fs)
      fs[0] = fibo_values[i-1];
#pragma omp task shared(fs)
      fs[1] = fibo_values[i-2];
#pragma omp taskwait
      fibo_values[i] = fs[0]+fs[1];
      }
  }
  printf("F(%d) = %ld\n",N,fibo_values[N-1]);

  return 0;
}
