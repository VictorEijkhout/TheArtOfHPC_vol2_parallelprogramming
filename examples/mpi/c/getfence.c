/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% getfence.c : MPI_Get with fences
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <mpi.h>
#include "window.c"

int main(int argc,char **argv) {

#include "globalinit.c"

  MPI_Win the_window;
  int my_number, other = nprocs-1;

  int *number_buffer = NULL;
  MPI_Alloc_mem
    ( /* size: */ 2*sizeof(int),
      MPI_INFO_NULL,&number_buffer);
  MPI_Win_create
    ( number_buffer,2*sizeof(int),sizeof(int),
      MPI_INFO_NULL,comm,&the_window);

  if (procno==other)
    number_buffer[1] = 27;
  test_window(the_window,comm);

  MPI_Win_fence(0,the_window);
  if (procno==0) {
    MPI_Get( /* data on origin: */   &my_number, 1,MPI_INT,
	     /* data on target: */   other,1,    1,MPI_INT,
	     the_window);
  }
  MPI_Win_fence(0,the_window);
  if (procno==0)
    printf("I got the following: %d\n",my_number);

  MPI_Win_free(&the_window);
  MPI_Free_mem(number_buffer);

  MPI_Finalize();
  return 0;
}
