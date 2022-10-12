/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% randomupdatelock.c : put random data everywhere, 
   %%%%    using exclusive locking
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
  srand( procno+1 );

  /*
   * Make a window exposing my table
   */
  int mycounter = 0;
  MPI_Win the_window;
  MPI_Aint winsize = sizeof(int), eltsize = sizeof(int);
  MPI_Win_create
    ( &mycounter,winsize,eltsize,MPI_INFO_NULL,comm,&the_window );
  int updates[nprocs];
  for (int p=0; p<nprocs; p++)
    updates[p] = 0;

  /*
   * Every process has an indididual number of writes to do
   */
  int howmanywrites = rand() % nprocs;
  printf("[%3d] going to do %d updates\n",procno,howmanywrites);
  /*
   * Acquire a lock for the full duration
   */
  int assert = 0; // MPI_MODE_NOCHECK;
  for (int iwrite=0; iwrite<howmanywrites; iwrite++) {

    /*
     * Who are you writing to in this iteration?
     */
    float randomfraction = (rand() / (double)RAND_MAX);
    int other_process = (int) ( nprocs * randomfraction );
    //printf("[%3d] updating %d\n",procno,other_process);
    updates[other_process] += 1;

    /*
     * Exercise:
     * - add 1 to the stack pointer on the other process
     * - which routine do you use? the window is locked so maybe put/get?
     */
    int lock_assert = 0;
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE, other_process,lock_assert,the_window);
    MPI_Aint zero_disp = 0; int increment_by_1=1;
    int readout;
    //MPI_Get( &readout,1,MPI_INT, other_process,zero_disp,1,MPI_INT, the_window );
    MPI_Accumulate
      ( &increment_by_1,1,MPI_INT,
        other_process,zero_disp,1,MPI_INT,
	MPI_SUM,the_window );
    MPI_Win_unlock(other_process, the_window);
    //printf("[%3d] finding %d on %d\n", procno,readout,other_process);
    
  }
  MPI_Win_free(&the_window);

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

  MPI_Finalize();
  return 0;
}
