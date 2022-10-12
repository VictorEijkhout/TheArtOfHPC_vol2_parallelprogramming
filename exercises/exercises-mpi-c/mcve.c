/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% randomupdateshared.c : put random data everywhere
   %%%%     using a shared lock
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#include "tools.h"

/*
 * Return max elementwise relative error
 */
double array_error(double ref_array[],double value_array[],int array_size) {
  double error = 0.,max_value=-1,min_value=-1;
  for (int i=0; i<array_size; i++) {
    double ref = ref_array[i], val = fabs(value_array[i]);
    if (min_value<0 || val<min_value) min_value = val;
    if (max_value<0 || val>max_value) max_value = val;
    double
      rel = (ref - val)/ref,
      e = fabs(rel);
    if (e>error) error = e;
  }
  return error;
};

/*
 * Return max elementwise error
 * for integer arrays
 */
int int_array_error(int ref_array[],int value_array[],int array_size) {
  int error = 0.,max_error=0;
  for (int i=0; i<array_size; i++) {
    int ref = ref_array[i], val = value_array[i],
      e = abs(val-ref);
    if (e>max_error) max_error = e;
  }
  return max_error;
};

/*
 * all processes have 1 integer, are they all the same?
 */
int test_all_the_same_int( int value,MPI_Comm comm ) {
  int final_min,final_max;
  MPI_Allreduce(&value,&final_min,1,MPI_INT,MPI_MIN,comm);
  MPI_Allreduce(&value,&final_max,1,MPI_INT,MPI_MAX,comm);
  return final_min==final_max;
}

/*
 * Every process has an error condition;
 * return lowest process no where condition true
 */
void print_final_result( int cond,MPI_Comm comm ) {
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  int error=nprocs, error_proc=-1;
  if (cond) 
    error = procno;
  MPI_Allreduce(&error,&error_proc,1,MPI_INT,MPI_MIN,comm);
  error_process_print(error_proc,comm);
};

void error_process_print(int error_proc, MPI_Comm comm) {
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  if (procno==0) {
    if (error_proc==nprocs) 
      printf("Finished; all results correct\n");
    else
      printf("First error occurred on proc %d\n",error_proc);
  }
}


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
   * and init to zero
   */
  void *baseptr;
  MPI_Win the_window;
  MPI_Aint winsize = sizeof(int), eltsize = sizeof(int);
  MPI_Win_allocate
    ( winsize,eltsize,MPI_INFO_NULL,comm,
      (void*)&baseptr, &the_window );
  /****************************************************************
   * REMOVE THE FOLLOWING BLOCK TO SEE UB/BUG
   ****************************************************************/
  {
    int mycounter=0;
    MPI_Win_fence(0,the_window);
    MPI_Aint zero_disp = 0;
    MPI_Put( &mycounter,1,MPI_INT,
	     procno,zero_disp,1,MPI_INT,
	     the_window);
    MPI_Win_fence(0,the_window);
  }
  /****************************************************************
   * UP TO HERE
   ****************************************************************/

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
  MPI_Win_lock_all(MPI_LOCK_SHARED, the_window);
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
    MPI_Aint zero_disp = 0;
    int increment = 1,readout;
    MPI_Accumulate
      ( &increment,1,MPI_INT,
        other_process,zero_disp,1,MPI_INT,
	MPI_SUM,the_window );
    /* MPI_Fetch_and_op( &increment,&readout,MPI_INT, */
    /* 		      other_process,zero_disp,MPI_SUM, */
    /* 		      the_window); */
    MPI_Win_flush_local(other_process,the_window);

  }
  MPI_Win_unlock_all(the_window);

  int mycounter;
  { // read out the local window content
    MPI_Win_fence(0,the_window);
    MPI_Aint zero_disp = 0;
    MPI_Get( &mycounter,1,MPI_INT,
	     procno,zero_disp,1,MPI_INT,
	     the_window);
    MPI_Win_fence(0,the_window);
    MPI_Win_free(&the_window);
    //printf("[%d] local counter=%d\n",procno,mycounter);
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

  MPI_Finalize();
  return 0;
}
