/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2024
   %%%%
   %%%% procname.cxx : processor name
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;

#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  //codesnippet mplprocname
  procno = comm_world.rank();
  string procname =
    mpl::environment::processor_name();
  stringstream ss;
  ss << "[" << procno << "] "
     << " Running on: " << procname;
  cout << ss.str() << '\n';
  //codesnippet end

  return EXIT_SUCCESS;

}
