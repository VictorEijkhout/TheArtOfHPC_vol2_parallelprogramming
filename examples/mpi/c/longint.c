/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2019-2022
   %%%%
   %%%% longint.c : the strange LONG_INT type
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#include "tools.h"

int main(int argc,char **argv) {

  MPI_Comm comm;
  MPI_Init(&argc,&argv); 
  comm = MPI_COMM_WORLD;

  int procno=-1,nprocs,err;
  MPI_Comm_rank(comm,&procno); 
  MPI_Comm_size(comm,&nprocs); 

  if (procno==0) {
    int s;
    MPI_Type_size( MPI_LONG_INT,&s );
    printf("MPI_LONG_INT size=%d\n",s);
    MPI_Aint ss;
    MPI_Type_extent( MPI_LONG_INT,&ss );
    printf("MPI_LONG_INT extent=%ld\n",ss);
  }
  
  MPI_Finalize();
  return 0;
}
