/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-8
   %%%%
   %%%% putfence.c : point-to-point through put & fence
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char **argv) {

#include "globalinit.c"

  MPI_Win the_window;
  int my_number=0, window_data[2], other = nprocs-1;
  if (procno==0)
    my_number = 37;

  MPI_Win_create
    (&window_data,2*sizeof(int),sizeof(int),
     MPI_INFO_NULL,comm,&the_window);
  MPI_Win_fence(0,the_window);
  if (procno==0) {
    MPI_Put
      ( /* data on origin: */   &my_number, 1,MPI_INT,
        /* data on target: */   other,1,    1,MPI_INT,
        the_window);
  }
  MPI_Win_fence(0,the_window);
  if (procno==other)
    printf("I got the following: %d\n",window_data[1]);
  MPI_Win_free(&the_window);

  MPI_Finalize();
  return 0;
}
