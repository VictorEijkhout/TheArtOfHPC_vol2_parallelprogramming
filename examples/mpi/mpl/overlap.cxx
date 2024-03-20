/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2024
   %%%%
   %%%% overlap.cxx : error by overlapping buffer
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
  mpl::contiguous_layout<double> v_layout(v.size()-1);

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
    try {
      comm_world.sendrecv // send to rank 1
	(v.data(), v_layout, 1, mpl::tag_t(0),
	 v.data()+1, v_layout, comm_world.size(), mpl::tag_t(0) );
    } catch(...) {
      std::cout << "process zero failed\n"; 
      return 1;
    }
    std::cout << "sent: ";
    for (double &x : v) 
      std::cout << x << ' ';
    std::cout << '\n';

  } else if (comm_world.rank()==1) {

    /*
     * Receive data and report
     */
    try {
      comm_world.sendrecv // send to rank 1
	(v.data(), v_layout, 0, mpl::tag_t(0),
	 v.data()+1, v_layout, -2, mpl::tag_t(0) );
    } catch(...) {
      std::cout << "process one failed\n"; 
      return 1;
    }
    std::cout << "got : ";
    for (double &x : v) 
      std::cout << x << ' ';
    std::cout << '\n';

  }
  return EXIT_SUCCESS;
}

