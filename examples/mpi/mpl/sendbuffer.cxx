/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% sendbuffer.cxx : send a buffer in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <iostream>
#include <vector>
#include <mpl/mpl.hpp>
 
int main() {
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  if (comm_world.size()<2)
    return EXIT_FAILURE;

  /*
   * To send a std::vector we declare a contiguous layout
   */
  std::vector<double> v(8);
  mpl::contiguous_layout<double> v_layout(v.size());

  // Initialize the data
  if (comm_world.rank()==0) {
    double init=0;
    for (double &x : v) {
      x=init;
      ++init;
    }
    
    /*
     * Send and report
     */
    comm_world.send(v.data(), v_layout, 1);  // send to rank 1
    std::cout << "sent: ";
    for (double &x : v) 
      std::cout << x << ' ';
    std::cout << '\n';

  } else if (comm_world.rank()==1) {

    /*
     * Receive data and report
     */
    comm_world.recv(v.data(), v_layout, 0);  // receive from rank 0
    std::cout << "got : ";
    for (double &x : v) 
      std::cout << x << ' ';
    std::cout << '\n';

  }
  return EXIT_SUCCESS;
}

