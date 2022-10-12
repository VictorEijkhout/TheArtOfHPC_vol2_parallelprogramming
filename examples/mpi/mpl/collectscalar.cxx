/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% collectscalar.cxx : collective routines on scalars in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <complex>
#include <iostream>
#include <vector>
#include <mpl/mpl.hpp>
 
int main() {

  // MPI Comm world
  const mpl::communicator &comm_world=mpl::environment::comm_world();

  // vector of consecutive floats
  std::vector<float> v;
  if (comm_world.rank()==0)
    for (int i=0; i<comm_world.size(); ++i)
      v.push_back(i);

  // if you scatter, everyone gets a number equal to their rank.
  // rank 0 scatters data to all processes
  float x;
  comm_world.scatter(0, v.data(), x);
  std::cout << "rank " << comm_world.rank() << " got " << x << '\n';
  
  // wait until all processes have reached this point
  comm_world.barrier();

  // multiply that number, giving twice your rank
  x*=2;
  
  // rank 0 gathers data from all processes
  comm_world.gather(0, x, v.data());
  if (comm_world.rank()==0) {
    std::cout << "got";
    for (int i=0; i<comm_world.size(); ++i)
      std::cout << " " << i << ":" << v[i];
    std::cout << std::endl;
  }

  // wait until all processes have reached this point
  comm_world.barrier();

  // calculate global sum and pass result to rank 0
  if (comm_world.rank()==0) {
    float sum;
    comm_world.reduce(mpl::plus<float>(), 0, x, sum);
    std::cout << "sum = " << sum << '\n';
  } else
    comm_world.reduce(mpl::plus<float>(), 0, x);

  // wait until all processes have reached this point
  comm_world.barrier();

  // calculate global sum and pass result to all
  {
    float
      xrank = static_cast<float>( comm_world.rank() ),
      xreduce;
    // separate recv buffer
    comm_world.allreduce(mpl::plus<float>(), xrank,xreduce);
    // in place
    comm_world.allreduce(mpl::plus<float>(), xrank);
    if ( comm_world.rank()==comm_world.size()-1 )
      std::cout << "Allreduce got: separate=" << xreduce
                << ", inplace=" << xrank << std::endl;
  }

  // calculate global sum and pass result to root
  {
    int root = 1;
    float
      xrank = static_cast<float>( comm_world.rank() ),
      xreduce;
    // separate receive buffer
    comm_world.reduce(mpl::plus<float>(), root, xrank,xreduce);
    // in place
    comm_world.reduce(mpl::plus<float>(), root, xrank);
    if ( comm_world.rank()==root )
      std::cout << "Allreduce got: separate=" << xreduce
		<< ", inplace=" << xrank << std::endl;
  }

  return EXIT_SUCCESS;
}

