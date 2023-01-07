/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% nodecount : how many nodes and how many procs per node?
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main( int argc,char **argv ) {
  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;
  int procno,nprocs;
  MPI_Comm_size( comm,&nprocs );
  MPI_Comm_rank( comm,&procno );

  MPI_Comm node_comm;
  MPI_Comm_split_type( comm,MPI_COMM_TYPE_SHARED,procno,MPI_INFO_NULL,&node_comm);
  int rank_on_node,size_of_node;
  MPI_Comm_rank( node_comm,&rank_on_node );
  MPI_Comm_size( node_comm,&size_of_node );
  int head_node = (rank_on_node==0);
  int number_of_nodes;
  MPI_Reduce( &head_node,&number_of_nodes,1,MPI_INT,MPI_SUM,0,comm);
  if (procno==0)
    printf("There are %d nodes\n",number_of_nodes);

  MPI_Comm node_heads;
  MPI_Comm_split( comm,head_node,procno,&node_heads );
  int node_sizes[number_of_nodes];
  MPI_Gather( &size_of_node,1,MPI_INT, node_sizes,1,MPI_INT, 0,node_heads );
  if (procno==0) {
    printf("Node sizes:");
    for (int inode=0; inode<number_of_nodes; inode++)
      printf(" %d",node_sizes[inode]);
    printf("\n");
  }

  MPI_Finalize();
  return 0;
}
