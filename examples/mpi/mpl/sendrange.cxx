/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% sendrange.cxx : send an iterator range in MPL
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
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  if (comm_world.size()<2)
    return EXIT_FAILURE;

  vector<double> v(15);

  if (comm_world.rank()==0) {
    
    // initialize
    for ( auto &x : v ) x = 1.41;

    /*
     * Send and report
     */
    comm_world.send(v.begin(), v.end(), 1);  // send to rank 1

  } else if (comm_world.rank()==1) {

    /*
     * Receive data and report
     */
    comm_world.recv(v.begin(), v.end(), 0);  // receive from rank 0

    cout << "Got:";
    for ( auto x : v )
      cout << " " << x;
    cout << endl;
  }
  return EXIT_SUCCESS;
}

