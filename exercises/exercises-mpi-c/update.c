/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% onesidedbuild.c : put random data everywhere
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>

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

  // Create the table in fake shared memory
  MPI_Win the_window;
#define MAXFULL 100
  int mytable[MAXFULL];
  mytable[0] = 0; // set stack pointer to first open location

  /*
   * Make a window exposing my table
   */
  MPI_Aint winsize = MAXFULL*sizeof(int), eltsize = sizeof(int);
  MPI_Win_create
    ( mytable,winsize,eltsize,MPI_INFO_NULL,comm,&the_window );
  int updates[nprocs];
  for (int p=0; p<nprocs; p++)
    updates[p] = 0;

#define HOWMANYWRITES 4
  int targets[HOWMANYWRITES];
  for (int iwrite=0; iwrite<HOWMANYWRITES; iwrite++) {
    float randomfraction = (rand() / (double)RAND_MAX);
    int other_process = (int) ( nprocs * randomfraction );
    targets[iwrite] = other_process;
    updates[other_process] += 1;

    /*
     * - Lock the window on the other process
     * - Get first open location on the target and increment this
     * - Write the random number
     */
    MPI_Win_fence(0,the_window);
    //    printf("[%d] accessing %d\n",procno,other_process);

    /*
     * Exercise:
     * - add 1 to the stack pointer on the other process
     * - which routine do you use? the window is locked so maybe put/get?
     */
    int increment_by_1=1;
/**** your code here ****/
    MPI_Win_fence(0,the_window);

  }

  /*
   * Correctness test
   */
  int all_updates[nprocs];
  MPI_Reduce( updates, all_updates,nprocs,MPI_INT, MPI_SUM,0,comm);
  int all_windows[nprocs];
  MPI_Gather( mytable,1,MPI_INT, all_windows,1,MPI_INT, 0,comm);
  int error_proc = nprocs;
  if (procno==0) {
    printf("Updates performed:");
    for (int p=0; p<nprocs; p++)
      printf(" %3i",all_updates[p]);
    printf("\n");
    printf("Windows gathered :");
    for (int p=0; p<nprocs; p++) {
      printf(" %3i",all_windows[p]);
      if (all_updates[p]!=all_windows[p] && error_proc==nprocs)
	error_proc += p;
    }
    printf("\n");
  }
  error_process_print(error_proc,comm);

  MPI_Win_free(&the_window);

  MPI_Finalize();
  return 0;
}
