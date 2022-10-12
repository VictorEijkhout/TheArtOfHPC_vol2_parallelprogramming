#include <stdio.h>
#include <stdlib.h>

int myDiv(int x, int y)
{
  return 1/(x - y);
}

int main(void)
{
  int res, x = 5, y;
  for(y = 1; y < 10; y++){
    res = myDiv(x,y);
    printf("%d,%d,%d\n",x,y,res);
  }
  return 0;
}
