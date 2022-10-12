/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% randomupdatefence.c : put random data everywhere, with fence sync
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

#include "tools.h"

int main(int argc,char **argv) {

  MPI_Comm comm;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  int procno=-1,nprocs,err;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);

  // Initialize the random number generator
  srand((int)(procno*nprocs));

  // Create a counter in a window
  MPI_Win the_window;
  int mycounter = 0;

  /*
   * Make a window exposing my table
   */
  MPI_Aint winsize = sizeof(int), eltsize = sizeof(int);
  MPI_Win_create
    ( &mycounter,winsize,eltsize,MPI_INFO_NULL,comm,&the_window );
  int updates[nprocs];
  for (int p=0; p<nprocs; p++)
    updates[p] = 0;

#define HOWMANYWRITES 4
  for (int iwrite=0; iwrite<HOWMANYWRITES; iwrite++) {
    float randomfraction = (rand() / (double)RAND_MAX);
    int other_process = (int) ( nprocs * randomfraction );
    updates[other_process] += 1;

    /*
     * - Lock the window on the other process
     * - Get first open location on the target and increment this
     * - Write the random number
     */
    MPI_Win_fence(0,the_window);

    /*
     * Exercise:
     * - add 1 to the stack pointer on the other process
     * - which routine do you use? the window is locked so maybe put/get?
     */
    int increment_by_1=1;
#if 1
    // this is not how to do it
    int old_value;
    MPI_Get( &old_value,1,MPI_INT,
        other_process,0,1,MPI_INT,
	the_window );
    int new_value = old_value + increment_by_1;
    MPI_Put( &new_value,1,MPI_INT,
        other_process,0,1,MPI_INT,
	the_window );
#else
/**** your code here ****/
#endif
    MPI_Win_fence(0,the_window);

  }

  /*
   * Correctness test
   */
  int all_updates[nprocs];
  MPI_Reduce( updates, all_updates,nprocs,MPI_INT, MPI_SUM,0,comm);
  int all_counters[nprocs];
  MPI_Gather( &mycounter,1,MPI_INT, all_counters,1,MPI_INT, 0,comm);
  int error_proc = nprocs;
  if (procno==0) {
    printf("Updates performed:");
    for (int p=0; p<nprocs; p++)
      printf(" %3i",all_updates[p]);
    printf("\n");
    printf("Windows gathered :");
    for (int p=0; p<nprocs; p++) {
      printf(" %3i",all_counters[p]);
    }
    printf("\n");
    int error = int_array_error(all_counters,all_updates,nprocs);
    if (error==0)
      printf("Finished: updates were correct\n");
    else
      printf("Difference between updates and windows content: %d\n",error);
  }
  //  error_process_print(error_proc,comm);

  MPI_Win_free(&the_window);

  MPI_Finalize();
  return 0;
}
