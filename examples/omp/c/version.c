/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% version.c : OpenMP standard
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "omp.h"

int main(int argc,char **argv) {

  //codesnippet ompstandard
  int standard = _OPENMP;
  printf("Supported OpenMP standard: %d\n",standard);
  switch (standard) {
  case  201511: printf("4.5\n");
    break;
  case 201611: printf("Technical report 4: information about 5.0 but not yet mandated.\n");
    break;
  case 201811: printf("5.0\n");
    break;
  case 202011:
    printf("5.1\n");
    break;
  case 202111: printf("5.2\n");
    break;
  default:
    printf("Unrecognized version\n");
    break;
  }
  //codesnippet end

  return 0;
}
