/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2021
   %%%%
   %%%% collectbuffer.cxx : collective routines on buffers in MPL
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
  vector<float> rank2p2p1{ 2*xrank,2*xrank+1 },reduce2p2p1{0,0};
  mpl::contiguous_layout<float> two_floats(rank2p2p1.size());
  comm_world.allreduce
    (mpl::plus<float>(), rank2p2p1.data(),reduce2p2p1.data(),two_floats);
  if ( iprint )
    cout << "Got: " << reduce2p2p1.at(0) << ","
	 << reduce2p2p1.at(1) << endl;

  /*
   * Scatter one number to each proc
   */
  if (iprint) cout << "Scattering 0--p" << endl;

  vector<float> v;

  if (comm_world.rank()==0)
    for (int i=0; i<comm_world.size(); ++i)
      v.push_back(i);

  // if you scatter, everyone gets a number equal to their rank.
  // rank 0 scatters data to all processes
  float x;
  comm_world.scatter(0, v.data(), x);

  if (iprint)
    cout << "rank " << procno << " got " << x << '\n';
  
  /*
   * Scatter two numbers to each proc
   */

  if (iprint) cout << "Scatter 0--2p" << endl;

  vector<float> vrecv(2),vsend(2*nprocs);

  if (comm_world.rank()==0)
    for (int i=0; i<2*nprocs; ++i)
      vsend.at(i) = i;

  // rank 0 scatters data to all processes
  // if you scatter, everyone gets 2p,2p+1
  mpl::contiguous_layout<float> twonums(2);
  comm_world.scatter
    (0, vsend.data(),twonums, vrecv.data(),twonums );

  if (iprint)
    cout << "rank " << procno << " got "
         << vrecv[0] << "," << vrecv[1] << '\n';
  
  return 0;
  // multiply that number, giving twice your rank
  x*=2;
  
  // rank 0 gathers data from all processes
  comm_world.gather(0, x, v.data());
  if (comm_world.rank()==0) {
    cout << "got";
    for (int i=0; i<comm_world.size(); ++i)
      cout << " " << i << ":" << v[i];
    cout << endl;
  }

  // wait until all processes have reached this point
  comm_world.barrier();

  // calculate global sum and pass result to rank 0
  if (comm_world.rank()==0) {
    float sum;
    comm_world.reduce(mpl::plus<float>(), 0, x, sum);
    cout << "sum = " << sum << '\n';
  } else
    comm_world.reduce(mpl::plus<float>(), 0, x);

  // wait until all processes have reached this point
  comm_world.barrier();

  return EXIT_SUCCESS;
}

