/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% topo.c : aux stuff for topology
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include "mpi.h"
void print_topo( MPI_Comm comm ) {
  int world_topo_status;
  MPI_Topo_test(comm,&world_topo_status);
  switch (world_topo_status) {
  case MPI_UNDEFINED :
    printf("Topology undefined\n"); break;
  case MPI_CART :
    printf("Topology cart\n"); break;
  case MPI_GRAPH :
    printf("Topology graph\n"); break;
  case MPI_DIST_GRAPH :
    printf("Topology dist_graph\n"); break;
  default:
    printf("Topology is weird\n");
  }
}
