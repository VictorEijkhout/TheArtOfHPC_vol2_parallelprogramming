/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Programming for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% OMP example program: reductions with explicit combiner expresion
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdio.h>
#include <omp.h>

#define abs(x)   (x<0 ? -x : x)
#define LARGENUM 2147483647
#define N        1000000
int data[N];

// return the smallest magnitude among all the integers in data[N]
int find_min_abs()
{
  int i;
  int result = LARGENUM;

#pragma omp declare reduction\
  (minabs : int :							\
   omp_out = abs(omp_in) > omp_out ? omp_out : abs(omp_in) )		\
  initializer (omp_priv=LARGENUM)

#pragma omp parallel for reduction(minabs:result)

  for (i=0; i<N; i++) {
    if (abs(data[i]) < result) {
      result = abs(data[i]);
    }
  }

  return result;
}

int main() {
  int i;
  for (i=0; i<N; i++) data[i] = N+3-i;
  printf("min: %d\n",find_min_abs());
  return 0;
}
