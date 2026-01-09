/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% ring_1c.c : code with a parallelism bug
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/


#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define SOME_MAX 128
int main(int argc, char *argv[])  {
  int numprocs, rank, tag1=1, tag2=2, i;
  int prev_value, local_value, next_value, prev_rank, next_rank;

  /* MPI_Request reqs[SOME_MAX]; */
  MPI_Status stats[SOME_MAX];

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Some local computation to generate local_value
  local_value = rank;
 
  // Assume 1-D ring topology
  prev_rank = (rank + numprocs - 1) % numprocs; // Previous neighbor
  next_rank = (rank + 1) % numprocs;            // Next neighbor

  //
  // Send local_value to nearest neighbors 
  //
  if(rank == prev_rank){
    MPI_Send(&local_value,1,MPI_INT,next_rank,tag1,MPI_COMM_WORLD);
    MPI_Recv(&prev_value ,1,MPI_INT,prev_rank,tag1,MPI_COMM_WORLD,&stats[0]);
  }
  else if(rank == next_rank){
    MPI_Send(&local_value,1,MPI_INT,prev_rank,tag2,MPI_COMM_WORLD);
    MPI_Recv(&next_value ,1,MPI_INT,next_rank,tag2,MPI_COMM_WORLD,&stats[1]);
  }

  // Serialized Output -- doesn't actually guarantee order
  for(i=0; i<numprocs; i++){
    if(rank == i){
      printf("(My Rank %i): ",rank);
      printf("Received Previous Neighbor's Rank: %i ",prev_value);
      printf("Received Next Neighbor's Rank: %i\n",next_value);
      fflush(stdout); // Flush stdout buffer 
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}

