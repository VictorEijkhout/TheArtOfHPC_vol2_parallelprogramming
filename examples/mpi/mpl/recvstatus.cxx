/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% recvstatus.cxx : inspect status object
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <complex>
#include <iostream>
#include <mpl/mpl.hpp>
 
int main() {
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  if (comm_world.size()<2)
    return EXIT_FAILURE;
  // send and recieve a single floating point number
  if (comm_world.rank()==0) {
    double pi=3.14;
    comm_world.send(pi, 1);  // send to rank 1
    std::cout << "sent: " << pi << '\n';
  } else if (comm_world.rank()==1) {
    double pi=0;
    auto s = comm_world.recv(pi, 0);  // receive from rank 0
    int c = s.get_count<double>();
    std::cout << "got : " << c << " scalar(s): " << pi << '\n';
  }
  return EXIT_SUCCESS;
}

