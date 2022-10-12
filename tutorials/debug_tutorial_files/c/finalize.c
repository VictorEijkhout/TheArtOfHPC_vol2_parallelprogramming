/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% finalize.c : code with a parallelism bug
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

void loop_for_awhile(MPI_Comm comm,double *return_random) {
  int mytid,ntids;

  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);

  // Initialize the random number generator
  srand((int)(mytid*(double)RAND_MAX/ntids));

  /*
   * This loop as given will terminate correctly
   */
  for (int it=0; ; it++) {
    double randomnumber = ntids * ( rand() / (double)RAND_MAX );
    /*
     * Uncomment the next line. 
     * Can you figure out a way to not get a mess on your screen?
     */
    //printf("[%d] iteration %d, random %e\n",mytid,it,randomnumber);
    if (randomnumber>mytid && randomnumber<mytid+1./(ntids+1)) {
      *return_random = randomnumber;
      return;
    }
    /*
     * The problem will show if you uncomment the next line
     */
    //MPI_Barrier(comm);
  }
}

int main(int argc,char **argv) {
  MPI_Comm comm;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  double returned_number;
  loop_for_awhile(comm,&returned_number);

  MPI_Finalize();
  return 0;
}
