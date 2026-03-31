/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2020-2026
 ****
 **** MPI Exercise for the use of Comm_rank/size
 ****
 ****************************************************************/

#include <iostream>
#include <format>
using namespace std;
#include <mpl/mpl.hpp>

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  
  // Exercise part 1:
  // -- Use the 
  //    `size' and `rank' methods
  // -- Let each processor print out a message like
  //    "Hello from processor 7 out of 12"
  //    reporting its number and the total number.
  int nprocs, procno;
/**** your code here ****/

  std::cout << std::format(
/**** your code here ****/
			   ) << '\n';

  // Exercise part 2:
  // -- let only processs zero print out
  //    "There are 16 processes"
  //    reporting only the total number
/**** your code here ****/
  
  return 0;
}
