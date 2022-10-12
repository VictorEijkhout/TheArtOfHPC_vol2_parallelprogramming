/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-9
   %%%%
   %%%% MPI exercise for implementing shared memory through one-sided
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#include "tools.h"

int main(int argc,char **argv) {

  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;

  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  if (nprocs<2) {
    printf("Need at least 2 procs\n");
    MPI_Abort(comm,0);
  }

  // first set a unique random seed
  srand((int)(procno*(double)RAND_MAX/nprocs));

  {
    int counter_process = nprocs-1;
    int my_number;

    /*
     * Create a window.
     * We only need a nonzero size on the last process,
     * which we label the `counter_process'
     */
    MPI_Win the_window;
    int window_data;
    if (procno==counter_process) {
      window_data = nprocs-1;
      MPI_Win_create(&window_data,sizeof(int),sizeof(int),
		     MPI_INFO_NULL,comm,&the_window);
    } else {
      MPI_Win_create(&window_data,0,sizeof(int),
		     MPI_INFO_NULL,comm,&the_window);
    }
    
    /*
     * Loop forever:
     * - at random times update the counter on the counter process
     * - and read out the counter to see if we stop
     */
    int final_value=nprocs;
    for (int step=0; ; step++) {
      /*
       * Some dynamic condition to determine whether we 
       * update the global counter
       */
      float randomfraction = (rand() / (double)RAND_MAX);
      int i_must_update = randomfraction<.5/nprocs;
      MPI_Win_fence(0,the_window);
      if (i_must_update) {
	int minus_one=-1;
	//printf("[%d] count down in step %d!\n",procno,step);
	/*
	 * Exercise:
	 * - subtract one from the global counter
	 *   do you use MPI_Put or MPI_Accumulate?
	 */
/**** your code here ****/
      }
      /*
       * Exercise:
       * - Now read out the global counter to see if it's time to stop.
       *   Here is the code:
       *   MPI_Get
       *   ( // data on origin: //   &is_zero, 1,MPI_INT,
       *     // data on target: //   counter_process,0,   1,MPI_INT,
       *     the_window);
       * - The problem is having having your data synchronized.
       *   Where are you going to have Win_fence calls?
       *   Is the one at the start of the loop enough?
       */
      int is_zero;
/**** your code here ****/
      if (procno==counter_process)
	printf("Step: %d, counter at %d\n",step,window_data);
      if (is_zero<=0) {
	final_value = is_zero;
	break;
      }
    }

    int error_test = ! test_all_the_same_int(final_value,comm);
    if (error_test)
      printf("Failure to agree on the final value: %d\n",final_value);
    print_final_result(error_test,comm);

    MPI_Win_free(&the_window);
  }

  MPI_Finalize();
  return 0;
}
