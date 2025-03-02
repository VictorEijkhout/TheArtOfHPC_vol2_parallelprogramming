/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% cartdims.c : cartesian grid exploration
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#define PERIOD

#include <fmt/format.h>
using fmt::format,fmt::print;

#include <mpl/mpl.hpp>

void print_grid( int nprocs,mpl::cartesian_communicator::dimensions dimensions ) {
  int dim = dimensions.dimensionality();
  print("Grid of size {} d in {} dimensions:\n  ",nprocs,dim);
  for (int idim=0; idim<dim; idim++) {
    //codesnippet mpldimsgetdims
    int dsize = dimensions.size(idim);
    auto p = dimensions.periodicity(idim);
    //codesnippet end
    print("{:2}",dsize);
    if ( p==mpl::cartesian_communicator::periodic )
      print("p");
    if (idim<dim-1) printf(" x ");
  } print("\n");
}

int main(int argc,char **argv) {

  const mpl::communicator &comm_world=mpl::environment::comm_world();
  auto procno = comm_world.rank();
  auto nprocs = comm_world.size();
  MPI_Comm comm = comm_world.native_handle();

  const int dim = 3;
  if (procno==0)
    printf("dim=%d\n",dim);

  /*
   * Create 3D brick, non-periodic
   */
#if 1
  //codesnippet mpldimscreate
  mpl::cartesian_communicator::dimensions brick(3);
  brick = mpl::dims_create(nprocs,brick);
  //codesnippet end
#else
  // non-periodic, explicit creation
  mpl::cartesian_communicator::dimensions brick;
  // explicit creation
  for ( int idim=0; idim<dim; idim )
    brick.add
      ( dimensions[idim],
        mpl::cartesian_communicator::periodicity_tag
            {mpl::cartesian_communicator::non_periodic} );
#endif
  //codesnippet mplcartcreate
  mpl::cartesian_communicator cart_comm( comm_world,brick );
  //codesnippet end

  /*
   * Print non-periodic grid
   */
  if (procno==0) {
    // grid dimensions
    print("Hyperplane13n\n");
    //codesnippet mplcartgetdims
    mpl::cartesian_communicator::dimensions
      dimensions = cart_comm.get_dimensions();
    //codesnippet end
    print_grid( nprocs,dimensions );

    // Translation rank -> coord
    //codesnippet mplcartrankcoord
    for ( int ip=0; ip<nprocs; ip++ ) {
      mpl::cartesian_communicator::vector
	coord = cart_comm.coordinates(ip);
      print("[{:2}] coord: [",ip);
      for ( int id=0; id<dim; id++ )
	print("{},",coord[id]);
      print("]\n");
    //codesnippet end
    }
    printf(" .. hyperplane13n\n");
  }
#ifdef PERIOD
  /*
   * Periodic brick
   */
  //codesnippet mpldimscreatep
  mpl::cartesian_communicator::dimensions pbrick
    ( { mpl::cartesian_communicator::non_periodic,
        mpl::cartesian_communicator::periodic,
        mpl::cartesian_communicator::non_periodic } );
  pbrick = mpl::dims_create(nprocs,pbrick);
  //codesnippet end
  mpl::cartesian_communicator period_comm( comm_world,pbrick );

  /*
   * Print periodic grid
   */
  if (procno==0) {
    // grid dimensions
    printf("Hyperplane13p\n");
    mpl::cartesian_communicator::dimensions
      dimensions = period_comm.get_dimensions();
    print_grid( nprocs,dimensions );

    // Translation rank -> coord
    int *coord = (int*) malloc( dim*sizeof(int) );
    for ( int ip=0; ip<nprocs; ip++ ) {
      mpl::cartesian_communicator::vector
	coord = cart_comm.coordinates(ip);
      print("[{:2}] coord: [",ip);
      for ( int id=0; id<dim; id++ )
	print("{},",coord[id]);
      print("]\n");
    }
    print(" .. hyperplane13p\n");
  }
#endif

  /*
   * Shifted coordinates
   */
  if (procno==0) {
    auto dimensions = cart_comm.get_dimensions();
    auto dim1 = dimensions.size(1);
      if (dim1==1) {
	printf("Too few processes: need non-trivial dimensions[1]\n");
      } else {
      print("\nCartShift\n");

      print("Shifting process 0.\n");
      //codesnippet mplcartshift
      int pred,succ;
      mpl::shift_ranks shifted = cart_comm.shift
	( /* dim: */ 1,/* up: */ 1 );
      pred = shifted.source; succ = shifted.destination;
      //codesnippet end
      print("non-periodic: src={}, tgt={}\n",pred,succ);

      mpl::shift_ranks pshifted = period_comm.shift(1,1);
      pred = pshifted.source; succ = pshifted.destination;
      print("periodic: src={}, tgt={}\n",pred,succ);
    }
  }

#if 0
  /*
   * Subdimensions
   */
  {
    int remain[] = {1,0,1};
    int topo_type, hyperdim, dims[3], period[3], coords[3];
    MPI_Comm hyperplane;

    if (procno==0) printf("Hyperplane13\n");
    MPI_Cart_sub( cart_comm.native_handle(),remain,&hyperplane );
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


  MPI_Finalize();
#endif

  return 0;
}
