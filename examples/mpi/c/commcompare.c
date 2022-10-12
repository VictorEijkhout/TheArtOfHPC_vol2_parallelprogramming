/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% commcompare.c : comm dup and compare
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  int result;
  MPI_Comm copy = comm;
  MPI_Comm_compare(comm,copy,&result);
  printf("assign:    comm==copy: %d \n",
         result==MPI_IDENT);
  printf("            congruent: %d \n",
         result==MPI_CONGRUENT);
  printf("            not equal: %d \n",
         result==MPI_UNEQUAL);

  MPI_Comm_dup(comm,&copy);
  MPI_Comm_compare(comm,copy,&result);
  printf("duplicate: comm==copy: %d \n",
         result==MPI_IDENT);
  printf("            congruent: %d \n",
         result==MPI_CONGRUENT);
  printf("            not equal: %d \n",
         result==MPI_UNEQUAL);

  MPI_Finalize();
  return 0;
}
