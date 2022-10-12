/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% postwaitwin.c : active target synchronization with post/wait/etc
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

  {
    MPI_Win the_window;
    MPI_Group all_group,two_group;
    int my_number = 37, other_number,
      twotids[2],origin,target;

    MPI_Win_create(&other_number,1,sizeof(int),
                   MPI_INFO_NULL,comm,&the_window);
    if (procno>0 && procno<nprocs-1) goto skip;

    origin = 0; target = nprocs-1;

    MPI_Comm_group(comm,&all_group);
    if (procno==origin) {
      MPI_Group_incl(all_group,1,&target,&two_group);
      // access
      MPI_Win_start(two_group,0,the_window);
      MPI_Put( /* data on origin: */   &my_number, 1,MPI_INT,
               /* data on target: */   target,0,   1,MPI_INT,
       the_window);
      MPI_Win_complete(the_window);
    }

    if (procno==target) {
      MPI_Group_incl(all_group,1,&origin,&two_group);
      // exposure
      MPI_Win_post(two_group,0,the_window);
      MPI_Win_wait(the_window);
    }
    if (procno==target)
      printf("Got the following: %d\n",other_number);
    
    MPI_Group_free(&all_group);
    MPI_Group_free(&two_group);
  skip:
    MPI_Win_free(&the_window);
  }

  MPI_Finalize();
  return 0;
}
