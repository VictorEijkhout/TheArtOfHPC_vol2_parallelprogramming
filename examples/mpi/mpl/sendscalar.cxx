/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% sendscalar.cxx : send a scalar in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <complex>
#include <iostream>
using std::cout;
using std::endl;
#include <mpl/mpl.hpp>
 
int main() {
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  if (comm_world.size()<2)
    return EXIT_FAILURE;

  // send and receive a single floating point number
  if (comm_world.rank()==0) {
    double pi=3.14;
    comm_world.send(pi, 1);  // send to rank 1
    cout << "sent: " << pi << '\n';
  } else if (comm_world.rank()==1) {
    double pi=0;
    comm_world.recv(pi, 0);  // receive from rank 0
    cout << "got : " << pi << '\n';
  }
  return EXIT_SUCCESS;
}

