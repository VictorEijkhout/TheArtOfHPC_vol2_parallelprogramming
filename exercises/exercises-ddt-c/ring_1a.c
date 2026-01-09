/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of
   %%%% `Introduction to High-Performance Scientific Computing'
   %%%% by Victor Eijkhout, copyright 2012/3/4/5
   %%%%
   %%%% This book is distributed under a Creative Commons Attribution 3.0
   %%%% Unported (CC BY 3.0) license and made possible by funding from
   %%%% The Saylor Foundation \url{http://www.saylor.org}.
   %%%%
   %%%% simple deadlock or eager send
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdio.h>
#include "mpi.h"
#define SOME_MAX 128
int main(int argc, char *argv[])  {
  int numprocs, rank, tag1=1, tag2=2, tag3=3,tag4=4;
  int prev_value, local_value, next_value, prev_rank, next_rank;
  MPI_Request reqs[SOME_MAX];
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
  MPI_Send(&local_value,1,MPI_INT,next_rank,tag1,MPI_COMM_WORLD);
  MPI_Recv(&prev_value ,1,MPI_INT,prev_rank,tag2,MPI_COMM_WORLD,&stats[0]);
  MPI_Send(&local_value,1,MPI_INT,prev_rank,tag3,MPI_COMM_WORLD);
  MPI_Recv(&next_value ,1,MPI_INT,next_rank,tag4,MPI_COMM_WORLD,&stats[1]);

  // Serialized Output -- still doesn't guarantee order
  for (int i=0; i<numprocs; i++){
    if(rank == i){
      printf("(My Rank %i): ",rank);
      printf("Received Previous Neighbor's Rank: %i ",prev_value);
      printf("Received Next Neighbor's Rank: %i\n",next_value);
      fflush(stdout); // Flush stdout buffer 
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

  MPI_Finalize();
}

