/*
 * testing out something.
 *
 * should this be written as a reduction?
 */

#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

void foo(double x, double *y) {

    if (x > *y) {
#pragma omp critical
        *y = x/2;
    }

}

#define N 5000
int main() {
  int x[N];
  for (int i=0; i<N; i++)
    x[i] = i;

  double y=0;
#pragma omp parallel for
  for ( int i  = 0; i<=N; i++) {
    for ( int j = 0; j<=N; j++) {
      foo(x[i], &y);
    }
  }
  return 0;
}

