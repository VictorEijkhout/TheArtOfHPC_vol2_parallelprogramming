/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2026
 ****
 **** MPI Exercise for Isend/Irecv using Cartesian topology
 ****
 ****************************************************************/

#include <iostream>
using std::cout;
#include <sstream>
#include <format>
using std::format;

#include <mpl/mpl.hpp>

#include "tools.h"

void print_grid( int nprocs,mpl::cartesian_communicator::dimensions dimensions ) {
  int dim = dimensions.dimensionality();
  cout << format("Grid of size {} d in {} dimensions:\n  ",nprocs,dim);
  for (int idim=0; idim<dim; idim++) {
    int dsize = dimensions.size(idim);
    auto p = dimensions.periodicity(idim);
    cout << format("{:2}",dsize);
    if ( p==mpl::cartesian_communicator::periodic )
      cout << format("p");
    if (idim<dim-1) cout << format(" x ");
  } cout << format("\n");
}

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  /*
   * Set up a non-periodic Cartesian grid
   */
  mpl::cartesian_communicator::dimensions dimensions(2);
  dimensions = mpl::dims_create(nprocs,dimensions);
  if (procno==0)
    print_grid( nprocs,dimensions );
  mpl::cartesian_communicator cart_comm( comm_world,dimensions );

  // Local data and space for receiving neighbor data
  double mydata=1,
    neighbor_data[4] = {0,0,0,0};
  // Count how many borders we are adjacent to
  int border_count = 0;
  mpl::irequest_pool requests;

  // Exercise:
  // -- set `successor' and `predecessor' for all four directions
  //    note that MPI_Cart_shift gives you two neighbors
  /*
   * First horizontal
   */
  int left,right;
  mpl::shift_ranks horizontal = cart_comm.shift
    (
/**** your code here ****/
     );
  left = horizontal.source; right = horizontal.destination;
  border_count += 
    ( left==MPI_PROC_NULL ? 1 : 0 ) +
    ( right==MPI_PROC_NULL ? 1 : 0 );
  
  // Data to the right
  requests.push( comm_world.isend(mydata,right) );
  requests.push( comm_world.irecv( neighbor_data[0],left ) );

  // Data to the left
/**** your code here ****/

  /*
   * Now vertical
   */
  int down,up;
  mpl::shift_ranks vertical = cart_comm.shift
    (
/**** your code here ****/
     );
  down = vertical.source; up = vertical.destination;
  border_count += 
    ( down==MPI_PROC_NULL ? 1 : 0 )
    + ( up==MPI_PROC_NULL ? 1 : 0 );
  
  // Data up
/**** your code here ****/
  
  // Data down
/**** your code here ****/

  //
  // Now make sure all Isend/Irecv operations are completed
  //
  requests.waitall();
  
  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  // check correctness
  mydata = mydata+neighbor_data[0]+neighbor_data[1]+neighbor_data[2]+neighbor_data[3];

  double res;
  if (border_count==2) {
    res = 3;
  } else if (border_count==1) {
    res = 4;
  } else {
    res = 5;
  }

  int error_test = not isapprox(mydata,res);
  if (error_test)
    cout << format("Data on proc {} should be {}, found {}\n",procno,res,mydata);
  print_final_result(error_test,comm_world);

  return 0;
}
