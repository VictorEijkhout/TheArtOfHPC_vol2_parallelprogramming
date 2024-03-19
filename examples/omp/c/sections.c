/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2024
   %%%%
   %%%% sections.c : omp sections
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>

#include "omp.h"

float f(float x) { return 1; }
float g(float x) { return 2; }
float h(float x) { return 3; }

int main(int argc,char **argv) {

  {
    float fx,gx,hx;
    //codesnippet ompsections1
    #pragma omp parallel sections
    {
      #pragma omp section
      fx = f(1.);
      #pragma omp section
      gx = g(1.);
      #pragma omp section
      hx = h(1.);
    }
    float s = fx+gx+hx;
    //codesnippet end
    printf("sum: %5.2f\n",s);
  }

  {
    //codesnippet ompsections2
    float s=0;
    #pragma omp parallel sections reduction(+:s)
    {
      #pragma omp section
      s += f(1.);
      #pragma omp section
      s += g(1.);
      #pragma omp section
      s += h(1.);
    }
    //codesnippet end
    printf("sum: %5.2f\n",s);
  }

  return 0;
}
