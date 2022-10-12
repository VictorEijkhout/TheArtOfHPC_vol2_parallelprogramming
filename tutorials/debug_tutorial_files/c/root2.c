#include <stdlib.h>
#include <stdio.h>
#include <math.h>

float root(float n)
{
  float r;
  r = sqrt(n);
  return r; 
}
 
int main() {
  float x,y;
  y = root(x);
  printf("root: %e\n",y);
  return 0;
}
