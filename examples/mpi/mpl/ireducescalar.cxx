/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% ireducescalar.cxx : non-blocking collective routines on scalars in MPL
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

  const mpl::communicator &comm_world =
    mpl::environment::comm_world();

  float x{1.},sum;
  auto reduce_request =
    comm_world.ireduce(mpl::plus<float>(), 0, x, sum);
  reduce_request.wait();
  if (comm_world.rank()==0) {
    std::cout << "sum = " << sum << '\n';
  }

  return EXIT_SUCCESS;
}

