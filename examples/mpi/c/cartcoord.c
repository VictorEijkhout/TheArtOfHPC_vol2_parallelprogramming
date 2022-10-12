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

void print_grid( int nprocs,int dim,int dimensions[] ) {
  printf("Grid of size %d in %d dimensions:\n  ",nprocs,dim);
  for (int idim=0; idim<dim; idim++) {
    printf("%2d",dimensions[idim]);
    if (idim<dim-1) printf(" x ");
  } printf("\n");
}

int main(int argc,char **argv) {

#include "globalinit.c"

  const int dim = 3;
  if (procno==0)
    printf("dim=%d\n",dim);

  /*
   * Create 3D brick
   */
  int *dimensions = (int*) malloc(dim*sizeof(int));
  for (int idim=0; idim<dim; idim++)
    dimensions[idim] = 0;
  MPI_Dims_create(nprocs,dim,dimensions);
  int *periods = (int*) malloc(dim*sizeof(int));
  for ( int id=0; id<dim; id++)
    periods[id] = 0;
  if (procno==0) {
    print_grid( nprocs,dim,dimensions );
  }
  MPI_Comm cart_comm;
  MPI_Cart_create
    ( comm,dim,dimensions,periods,
      0,&cart_comm );
  MPI_Comm period_comm;
  for ( int id=0; id<dim; id++)
    periods[id] = id==0 ? 1 : 0;
  MPI_Cart_create
    ( comm,dim,dimensions,periods,
      0,&period_comm );

  /*
   * Translation rank -> coord
   */
  if (procno==0) {
    int *coord = (int*) malloc( dim*sizeof(int) );
    for ( int ip=0; ip<nprocs; ip++ ) {
      /*
       * Translate process rank to cartesian coordinate
       */
      MPI_Cart_coords( cart_comm,ip,dim,coord );
      printf("[%2d] coord: [",ip);
      for ( int id=0; id<dim; id++ )
	printf("%d,",coord[id]);
      printf("]\n");

      /*
       * Shift the coordinate and translate back to rank
       * This is erroneous for a non-periodic Cartesian grid
       */
      int rank_check;
      coord[0]++;
      MPI_Cart_rank( cart_comm,coord,&rank_check );
      printf("   shifted neighbor : %2d\n",rank_check);
      MPI_Cart_rank( period_comm,coord,&rank_check );
      printf("   periodic neighbor: %2d\n",rank_check);
    }
    free(coord);
  }

  /*
   * Shifted coordinates
   */
  if (procno==0) {
    if (dimensions[1]==1) {
      printf("Too few processes: need non-trivial dimensions[1]\n");
    } else {
      printf("\nCartShift\n");
      print_grid(nprocs,dim,dimensions);
      printf("Shifting process 0.\n");
      int pred,succ;
      MPI_Cart_shift
	(period_comm,/* dim: */ 0,/* up: */ 1,
	 &pred,&succ);
      printf("periodic dimension 0:\n  src=%d, tgt=%d\n",
             pred,succ);
      MPI_Cart_shift
	(period_comm,/* dim: */ 1,/* up: */ 1,
	 &pred,&succ);
      printf("non-periodic dimension 1:\n  src=%d, tgt=%d\n",
             pred,succ);
      printf("cartshift\n\n");
    }
  }

  /*
   * Subdimensions
   */
  {
    int remain[] = {1,0,1};
    int topo_type, hyperdim, dims[3], period[3], coords[3];
    MPI_Comm hyperplane;

    if (procno==0) printf("Hyperplane13\n");
    MPI_Cart_sub( cart_comm,remain,&hyperplane );
    if (procno==0) {
      MPI_Topo_test( hyperplane,&topo_type );
      MPI_Cartdim_get( hyperplane,&hyperdim );
      printf("hyperplane has dimension %d, type %d\n",
             hyperdim,topo_type);
      MPI_Cart_get( hyperplane,dim,dims,period,coords );    
      printf("  periodic: ");
      for (int id=0; id<2; id++)
        printf("%d,",period[id]);
      printf("\n");
    }
    MPI_Comm_free( &hyperplane );
    if (procno==0) printf("hyperplane13\n\n");

    if (procno==0) printf("Hyperplane13p\n");
    MPI_Cart_sub( period_comm,remain,&hyperplane );
    if (procno==0) {
      MPI_Topo_test( hyperplane,&topo_type );
      MPI_Cartdim_get( hyperplane,&hyperdim );
      printf("hyperplane has dimension %d, type %d\n",
             hyperdim,topo_type);
      MPI_Cart_get( hyperplane,dim,dims,period,coords );    
      printf("  periodic: ");
      for (int id=0; id<2; id++)
        printf("%d,",period[id]);
      printf("\n");
    }
    MPI_Comm_free( &hyperplane );
    if (procno==0) printf("hyperplane13p\n\n");

  }

  free(dimensions); free(periods);

  MPI_Finalize();
  return 0;
}
