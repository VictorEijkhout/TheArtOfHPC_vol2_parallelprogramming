/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% MPI exercise for implementing shared memory through one-sided
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <mpi.h>
#include "gather_sort_print.h"

int main(int argc,char **argv) {

  int nprocs,procno;
  MPI_Init(0,0);
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  if (nprocs<2) {
    printf("Need at least 2 procs\n");
    MPI_Abort(comm,0);
  }

  // first set a unique random seed
  srand(procno*time(0));

  {
    /*
     * Create a window.
     * We only need a nonzero size on the last process,
     * which we label the `counter_process';
     * everyone else makes a window of size zero.
     */
    MPI_Win the_window;
    int counter_process = nprocs-1;
    int window_data,check_data;
    if (procno==counter_process) {
      window_data = 2*nprocs-1;
      check_data = window_data;
      MPI_Win_create(&window_data,sizeof(int),sizeof(int),
                     MPI_INFO_NULL,comm,&the_window);
    } else {
      MPI_Win_create(&window_data,0,sizeof(int),
                     MPI_INFO_NULL,comm,&the_window);
    }
    /*
     * Initialize the window
     * - PROCWRITES is approx the number of writes we want each process to do
     * - COLLISION  is approx how many processes will collide on a write
     */
#ifndef COLLISION
#define COLLISION 2
#endif
#ifndef PROCWRITES
#define PROCWRITES 40
#endif
    int counter_init = nprocs * PROCWRITES;
    //codesnippet countdowninitput
    MPI_Win_fence(0,the_window);
    if (procno==counter_process)
      MPI_Put(&counter_init,1,MPI_INT,
              counter_process,0,1,MPI_INT,
              the_window);
    MPI_Win_fence(0,the_window);
    //codesnippet end
    
    /*
     * Allocate an array (grossly over-dimensioned)
     * for the counter values that belong to me
     */
    int *my_counter_values = (int*) malloc( counter_init * sizeof(int) );
    if (!my_counter_values) {
      printf("[%d] could not allocate counter values\n",procno);
      MPI_Abort(comm,0);
    }
    int n_my_counter_values = 0;

    /*
     * Loop:
     * - at random times update the counter on the counter process
     * - and read out the counter to see if we stop
     */
    int total_decrement = 0;
    int nsteps = PROCWRITES / COLLISION;
    if (procno==0)
      printf("Doing %d steps, counter starting: %d\n    probably %d-way collision on each step\n",
             nsteps,counter_init,COLLISION);
    for (int step=0; step<nsteps ; step++) {

      /*
       * Basic probability of a write is 1/P,
       * so each step only one proc will write.
       * Increase chance of collision by upping 
       * the value of COLLISION.
       */
      float randomfraction = (rand() / (double)RAND_MAX);
      int i_am_available = randomfraction < ( COLLISION * 1./nprocs );

      /*
       * Exercise:
       * - atomically read and decrement the counter
       */
      //codesnippet fetchopfence
      MPI_Win_fence(0,the_window);
      int 
        counter_value;
      if (i_am_available) {
        int
          decrement = -1;
        total_decrement++;
        MPI_Fetch_and_op
          ( /* operate with data from origin: */   &decrement,
            /* retrieve data from target:     */   &counter_value,
            MPI_INT, counter_process, 0, MPI_SUM,
            the_window);
        //codesnippet end
#ifdef DEBUG
        printf("[%d] updating in step %d; retrieved %d\n",procno,step,counter_value);
#endif
        //codesnippet fetchopfence
      }
      MPI_Win_fence(0,the_window);
      if (i_am_available) {
        my_counter_values[n_my_counter_values++] = counter_value;
      }
      //codesnippet end
    }

    /*
     * What counter values were actually obtained?
     */
    gather_sort_print( my_counter_values,n_my_counter_values, comm );

    /*
     * We do a correctness test by computing what the 
     * window_data is supposed to be
     */
    {
      MPI_Win_fence(0,the_window);
      int counter_value;
      MPI_Get( /* origin data to set: */ &counter_value,1,MPI_INT,
               /* window data to get: */ counter_process,0,1,MPI_INT,
               the_window);
      MPI_Win_fence(0,the_window);
      MPI_Allreduce(MPI_IN_PLACE,&total_decrement,1,MPI_INT,MPI_SUM,comm);
      if (procno==counter_process) {
        if (counter_init-total_decrement==counter_value)
          printf("[%d] initial counter %d decreased by %d correctly giving %d\n",
                 procno,counter_init,total_decrement,counter_value);
        else
          printf("[%d] initial counter %d decreased by %d, giving %d s/b %d\n",
                 procno,counter_init,total_decrement,counter_value,counter_init-total_decrement);
      }
    }
    MPI_Win_free(&the_window);
  }

  MPI_Finalize();
  return 0;
}
