/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% race.c : race condition illustration
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

int f(int counter,int i) {
  return 1;
};

int main(int argc,char **argv) {

  int count = 100000;
  int counter = 0;

  //codesnippet racecounter
#pragma omp parallel for shared(counter)
  for (int i=0; i<count; i++)
    counter += f(counter,i);
  printf("Counter should be %d, is %d\n",
	 count,counter);
  //codesnippet end

  
  return 0;
}
