/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% collectarray.cxx : collective routines on static arrays in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <complex>
#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <mpl/mpl.hpp>
 
int main() {

  // MPI Comm world
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  int nprocs = comm_world.size(), procno = comm_world.rank();
  int iprint = procno==nprocs-1;

  /*
   * Reduce a 2 int buffer
   */
  if (iprint) cout << "Reducing 2p, 2p+1" << endl;

  float
    xrank = static_cast<float>( comm_world.rank() );
  //codesnippet mplallreducearray
  float rank2p2p1[2] = { 2*xrank,2*xrank+1 };
  mpl::contiguous_layout<float> p2layout(2);
  comm_world.allreduce(mpl::plus<float>(), rank2p2p1, p2layout);
  //codesnippet end
  if ( iprint )
    cout << "Got: " << rank2p2p1[0] << ","
	 << rank2p2p1[1] << endl;

  return EXIT_SUCCESS;
}

