/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2024
   %%%%
   %%%% bcastreduce.cxx : rooted collectives
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

  //codesnippet mplbcast
  int sendnum;
  int root = comm_world.size()-1;
  if ( comm_world.rank()==root )
    sendnum = 1;
  comm_world.bcast( root,sendnum );
  //codesnippet end

  int reducesum;
  if ( comm_world.rank()==root ) {
    comm_world.reduce
      ( mpl::plus<int>(), root,sendnum,reducesum );
    cout << "sum: " << reducesum << '\n';
  } else
    comm_world.reduce
      ( mpl::plus<int>(), root,sendnum );

  return EXIT_SUCCESS;
}

