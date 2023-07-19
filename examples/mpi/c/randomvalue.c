/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% randomvalue.c : illustrate SPMD by generating random random
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <mpi.h>

int main(int argc,char **argv) {

  MPI_Comm comm;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  int procno=-1,nprocs,err;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);

  // Initialize the random number generator
  srand( time(NULL) );

  // compute and print a random value
  int randomvalue = rand();
  //  printf("Random value: %d\n",randomvalue);
  double time = MPI_Wtime();
  printf("Process time: %e\n",time);

  {
    int *v,flag;
    MPI_Attr_get( comm,MPI_WTIME_IS_GLOBAL,&v,&flag);
    if (procno==0)
      printf("Time is synchronized? %d -> %d\n",
	     flag,*v);
  }

  MPI_Finalize();
  return 0;
}
