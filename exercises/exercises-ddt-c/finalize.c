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

  MPI_Comm_size(comm,&ntids);
  MPI_Comm_rank(comm,&mytid);

  /*
   * This loop as given will terminate correctly
   */
  for (int it=0; ; it++) {
    double randomnumber = ntids * ( rand() / (double)RAND_MAX );

    /*
     * Print some global statistics
     */
    {
      double maxrandom;
      MPI_Allreduce( &randomnumber,&maxrandom,1,MPI_DOUBLE,MPI_MAX,comm);
      if (mytid==0) printf("Max random number: %e\n",maxrandom);
    }
    /*
     * Exit if you have found a suitable number.
     *
     * Exercise:
     * -- what is wrong with this part of the code?
     */
    if (randomnumber>mytid && randomnumber<mytid+1./(ntids+1)) {
      *return_random = randomnumber;
      return;
    }
  }
}

int main(int argc,char **argv) {
  MPI_Comm comm;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  int mytid;
  MPI_Comm_rank(comm,&mytid);
  // Initialize the random number generator
  srand(mytid);
  
  double returned_number;
  loop_for_awhile(comm,&returned_number);
  printf("[%d] returned %e\n",mytid,returned_number);

  MPI_Finalize();
  return 0;
}
