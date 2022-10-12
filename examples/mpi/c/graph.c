/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% graph.c : illustrating a graph topology
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mpi.h"
#include "topo.c"

int main(int argc,char **argv) {

#include "globalinit.c"

  if (nprocs<4) {
    printf("This program needs at least four processes\n");
    return -1;
  }

  if (procno==0)
    print_topo(comm);

  int dimensions[2];
  dimensions[0] = 0; dimensions[1] = 0;
  MPI_Dims_create(nprocs,2,dimensions);
  int idim=dimensions[0],jdim=dimensions[1];
  if (procno==0)
    printf("Processor grid: %dx%d\n",idim,jdim);

  int proci,procj;
  proci = procno/jdim;
  procj = procno%jdim;
  printf("[%d] in grid at ( %2d,%2d )\n",procno,proci,procj);

#define PROC(i,j,I,J) ( (i+I)%I )*J + (j+J)%J
  MPI_Comm comm2d;
  {
    int degree=0;
    int destinations[4]={-1,-1,-1,-1};
    int weights[4] = {1,1,1,1};
    for ( int i=0; i<=1; i++ ) {
      int neighb_i = proci+i;
      if (neighb_i<0 || neighb_i>=idim)
        continue;
      int j = 1-i;
      int neighb_j = procj+j;
      if (neighb_j<0 || neighb_j>=jdim)
	continue;
      destinations[ degree++ ] = 
	PROC(neighb_i,neighb_j,idim,jdim);
    }
    MPI_Dist_graph_create
      (comm,
       /* I specify just one proc: me */ 1,
       &procno,&degree,destinations,weights,
       MPI_INFO_NULL,0,
       &comm2d
       );
    printf("Nbors\n");
    printf("[%2d = (%d,%d)] has %2d outbound:",procno,proci,procj,degree);
    for (int n=0; n<degree; n++)
      printf(" %d,",destinations[n]);
    printf("\n");
    printf("nbors\n");
    if (procno==0)
      print_topo(comm2d);
  }

  // statistics gathering
  printf(">> Nbors\n");
  {
    int indegree,outdegree,
      weighted;
    MPI_Dist_graph_neighbors_count
      (comm2d,
       &indegree,&outdegree,
       &weighted);
    int
      my_ij[2] = {proci,procj},
      other_ij[4][2];
    MPI_Neighbor_allgather
      ( my_ij,2,MPI_INT,
        other_ij,2,MPI_INT,
        comm2d );

    printf("  %2d inbound:",indegree);
    for (int in=0; in<indegree; in++) {
      int ni = other_ij[in][0], nj = other_ij[in][1];
      printf(" (%d,%d)=%d",ni,nj,PROC(ni,nj,idim,jdim));
    }
    printf("\n");
  }
  printf("<< nbors\n");

  //neighbor counting
  printf(">> Count\n");
  {
    int indegree=4, sources[indegree],
      inweights[indegree],weighted;
    int outdegree=4, targets[outdegree],
      outweights[outdegree];
    MPI_Dist_graph_neighbors_count
      (comm2d,
       &indegree,&outdegree,
       &weighted);
    MPI_Dist_graph_neighbors
      (comm2d,
       indegree,sources,inweights,
       outdegree,targets,outweights
       );
    printf("  %2d inbound:",indegree);
    for (int in=0; in<indegree; in++) {
      printf(" %d",sources[in]);
    }
    printf("\n");
  }
  printf("<< count\n");

  MPI_Finalize();
  return 0;
}
