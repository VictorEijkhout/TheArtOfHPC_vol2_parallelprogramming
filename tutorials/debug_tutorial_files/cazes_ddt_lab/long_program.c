//---------------------------------------------
// Example code for debugging training at TACC
//
// v1.0, 2018-06-27, dmcdougall@tacc.utexas.edu
//---------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double expensive_function(int N)
{
  double * a = calloc(N, sizeof(double));
  double * b = calloc(N, sizeof(double));
  double * c = calloc(N, sizeof(double));

  // Set up arrays
  for (int i = 0; i < N; i++) {
    a[i] = 2.0 * i;
    b[i] = 3.0 * i;
  }

  // Do some really hard work
  for (int i = 0; i < N; i++) {
    c[i] = sqrt(a[i]) + sin(b[i]);
  }

  return c[1];
}

int main(void)
{
  int N = 1000000000;
  double res = expensive_function(N);
  printf("%d,%lf\n",N,res);
  return 0;
}
