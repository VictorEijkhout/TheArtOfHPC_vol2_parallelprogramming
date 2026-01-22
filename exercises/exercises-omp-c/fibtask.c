/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2024
 ****
 **** fibtask.c : OpenMP Exercise for fibonacci with tasks
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

long fib(int n) {
  if (n<2) return n;
  else {
    long f1,f2;
#pragma omp task shared(f1)
    f1 = fib(n-1);
#pragma omp task shared(f2)
    f2 = fib(n-2);
#pragma omp taskwait
    return f1+f2;
  }
}

int main(int argc,char **argv) {

  int n=20;
  if (argc==2)
    n = atoi(argv[1]);

#pragma omp parallel
#pragma omp single
  printf("Fib(%d)=%ld\n",n,fib(n));

  return 0;
}
