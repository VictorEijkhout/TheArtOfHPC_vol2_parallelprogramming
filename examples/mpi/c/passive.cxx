/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% passive.cxx : using passive target synchronization
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
using namespace std;

int main(int argc,char **argv) {

#include "globalinit.c"

  // Create the table in fake shared memory
  MPI_Win the_window;
  int
    repository = 0, // process that keeps the repository
    ninputs = nprocs-1;
  float *inputs;

  if (procno==repository) {
    // Repository processor creates a table of inputs
    // and associates that with the window
    inputs = new float[ninputs];
    for (int i=0; i<ninputs; i++)
      inputs[i] = .01 * rand() / (float)RAND_MAX ;
    MPI_Win_create(inputs,ninputs*sizeof(float),sizeof(float),
		   MPI_INFO_NULL,comm,&the_window);
  } else {
    // for all other processors the window is null
    MPI_Win_create(NULL,0,sizeof(int),
		   MPI_INFO_NULL,comm,&the_window);
  }

  // bookkeeping: which jobs did I process?
  int *myjobs = new int[ninputs];
  for (int i=0; i<ninputs; i++) 
    myjobs[i] = 0;
  if (procno!=repository) {
    float contribution=(float)procno,table_element;
    int loc=0;
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE,repository,0,the_window);
    // read the table element by getting the result from adding zero
    MPI_Fetch_and_op
      (&contribution,&table_element,MPI_FLOAT,
       repository,loc,MPI_SUM,the_window);
    MPI_Win_unlock(repository,the_window);
  }

  MPI_Win_free(&the_window); 

  MPI_Barrier(comm);

  if (procno==repository) {
    printf("Final result: %e\n",inputs[0]);
  }

  MPI_Finalize();
  return 0;
}
