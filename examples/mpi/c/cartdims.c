/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% cartdims.c : cartesian grid exploration
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  for (int dim=1; dim<=nprocs; dim++) {
    if (procno==0) {
      printf("CartDims\n");
      printf("Cartesian grid size: %d dim: %d\n",nprocs,dim);
    }
    int *dimensions = (int*) malloc(dim*sizeof(int));
    for (int idim=0; idim<dim; idim++)
      dimensions[idim] = 0;
    MPI_Dims_create(nprocs,dim,dimensions);

    int final = ( dimensions[dim-1]==1 );
    //if (procno==0) printf("final: %d \n",final);

    if (procno==0) {
      for (int idim=0; idim<dim; idim++) {
	printf("%2d",dimensions[idim]);
	if (idim<dim-1) printf(" x ");
      } printf("\n");
      printf("cartdims\n");
    }

    MPI_Comm cart_comm;
    int *periods = (int*) malloc(dim*sizeof(int));
    for ( int id=0; id<dim; id++ ) periods[id] = 0;
    MPI_Cart_create
      ( comm,dim,dimensions,periods,
        0,&cart_comm );

    int world_type,cart_type;
    MPI_Topo_test( comm,&world_type);
    MPI_Topo_test( cart_comm,&cart_type );
    if (procno==0) {
      printf("World comm type=%d, Cart comm type=%d\n",
             world_type,cart_type);
      printf("no topo        =%d, cart top      =%d\n",
             MPI_UNDEFINED,MPI_CART);
    }

    if (cart_type==MPI_CART) {
      int dim;
      MPI_Cartdim_get( cart_comm,&dim );
      int *dimensions = (int*) malloc(dim*sizeof(int));
      int *periods    = (int*) malloc(dim*sizeof(int));
      int *coords     = (int*) malloc(dim*sizeof(int));
      MPI_Cart_get( cart_comm,dim,dimensions,periods,coords );
      printf("process %2d is [",procno);
      for (int id=0; id<dim; id++) printf("%d,",coords[id]);
      printf("]\n");
    } else { printf("Uh oh\n"); continue; }
    free(dimensions); free(periods);

    /*
     * if the last component was a 1
     * there is no point in continuing
     */
    if (final) break;
  }

  MPI_Finalize();
  return 0;
}
