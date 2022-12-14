/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-6
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

  //codesnippet windowsizeof
  MPI_Win the_window;
  int *window_data;
  MPI_Win_allocate(2*sizeof(int),sizeof(int),
		   MPI_INFO_NULL,comm,
		   &window_data,&the_window);
  //codesnippet end

  int my_number=0, other = nprocs-1;
  if (procno==0)
    my_number = 37;

  MPI_Win_fence(0,the_window);
  if (procno==0) {
    MPI_Put( /* data on origin: */   &my_number, 1,MPI_INT,
	     /* data on target: */   other,1,    1,MPI_INT,
	     the_window);
  }
  MPI_Win_fence(0,the_window);
  //codesnippet end
  if (procno==other)
    printf("I got the following: %d\n",window_data[1]);
  //codesnippet putblock
  MPI_Win_free(&the_window);
  //codesnippet end

  MPI_Finalize();
  return 0;
}
