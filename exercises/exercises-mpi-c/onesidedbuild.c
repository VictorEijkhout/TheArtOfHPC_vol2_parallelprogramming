/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
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

#define HOWMANYWRITES 2
  int targets[HOWMANYWRITES];
  for (int iwrite=0; iwrite<HOWMANYWRITES; iwrite++) {
    float randomfraction = (rand() / (double)RAND_MAX);
    int other_process = (int) ( nprocs * randomfraction );
    targets[iwrite] = other_process;

    /*
     * - Lock the window on the other process
     * - Get first open location on the target and increment this
     * - Write the random number
     */
    MPI_Win_fence(0,the_window);
    printf("[%d] accessing %d\n",procno,other_process);
    int increment_by_1=1,stack_pointer;
    MPI_Aint pointer_loc=0;
    /*
     * Exercise:
     * - add 1 to the stack pointer on the other process
     * - which routine do you use? the window is locked so maybe put/get?
     */
/**** your code here ****/
    MPI_Win_fence(0,the_window);
    printf("[%d] got stack pointer %d\n",procno,stack_pointer);

    /*
     * Exercise: write the number
     * - `stackpointer' now describes where you can safely write in the other process
     * - what call are you using to transfer data? are there any possible conflicts?
     */
    int number_for_other = procno;
    MPI_Aint offset = stack_pointer+1;
    printf("[%d] write to %d at %d\n",procno,other_process,offset);
/**** your code here ****/
    // And now we can unlock the window so that other processes can access it
    MPI_Win_fence(0,the_window);
      //    MPI_Win_unlock(other_process,the_window);
  }

  MPI_Win_free(&the_window);

  printf("[%d] wrote its id to:",procno);
  for (int iwrite=0; iwrite<HOWMANYWRITES; iwrite++)
    printf(" %d",targets[iwrite]);
  printf("\n");
  int nrecv = mytable[0];
  printf("[%d] received %d:",procno,nrecv);
  for (int iwrite=1; iwrite<=nrecv; iwrite++)
    printf(" %d",mytable[iwrite]);
  printf("\n");

  MPI_Barrier(comm);

  MPI_Finalize();
  return 0;
}
