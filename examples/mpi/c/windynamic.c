/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% windynamic.c : Use of dynamically created window
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

  {
    MPI_Win the_window;
    int origin=0, data_proc = nprocs-1;
    int *retrieve=NULL,*window_buffer=NULL;

    MPI_Win_create_dynamic(MPI_INFO_NULL,comm,&the_window);
    if (procno==data_proc)
      window_buffer = (int*) malloc( 2*sizeof(int) );
    MPI_Win_attach(the_window,window_buffer,2*sizeof(int));

    test_window( the_window,comm );

    if (procno==data_proc) {
      window_buffer[0] = 1;
      window_buffer[1] = 27;
    }
    if (procno==origin) {
      retrieve = (int*) malloc( sizeof(int) );
    }

    MPI_Aint data_address;
    if (procno==data_proc) {
      MPI_Get_address(window_buffer,&data_address);
    }
    MPI_Bcast(&data_address,1,MPI_AINT,data_proc,comm);

    MPI_Win_fence(0,the_window);
    if (procno==origin) {
      MPI_Aint disp = data_address+1*sizeof(int);
      MPI_Get( /* data on origin: */           retrieve, 1,MPI_INT,
       /* data on target: */ data_proc,disp,     1,MPI_INT,
       the_window);
    }
    MPI_Win_fence(0,the_window);

    if (procno==origin)
      printf("I got the following: %d\n",retrieve[0]);
    MPI_Win_free(&the_window);
  }

  MPI_Finalize();
  return 0;
}
