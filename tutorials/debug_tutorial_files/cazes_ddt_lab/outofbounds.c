//---------------------------------------------
// Example code for debugging training at TACC
//
// v1.0, 2015-06-24, carlos@tacc.utexas.edu
// v1.1, 2018-06-27, dmcdougall@tacc.utexas.edu
//---------------------------------------------
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
  int i;
  double *a, *b;

  a = (double *)malloc(5 * sizeof(double));
  b = (double *)malloc(5 * sizeof(double));

  for(i = 0; i < 5; i++){
    a[i] = 1;
    b[i] = 2;
  }

  // Array is accessing elements beyond allocated boundary
  for(i = 1; i <= 5; i++) {
    a[i] = 2 * b[i];
    printf("a[%d] = %e\n", i, a[i]);
  }

  free(a);
  free(b);
  return 0;
}
