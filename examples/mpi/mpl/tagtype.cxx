/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2023
   %%%%
   %%%% tagtype.cxx : tag type
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <mpl/mpl.hpp>
 
int main() {

  /*
   * Ensure we have two processes
   */
  const mpl::communicator &
    world(mpl::environment::comm_world());
  if (world.size()<2)
    world.abort(EXIT_FAILURE);

  double x[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int pinger = 0, ponger = world.size()-1;
  //codesnippet mpltagtyp
  auto tag = mpl::tag_t(25);
  if (world.rank()==pinger) {
    world.send(x, ponger, tag );
    world.recv(x, ponger, tag );
  } else if (world.rank()==ponger) {
    world.recv(x, pinger, tag );
    world.send(x, pinger, tag );
  }
  //codesnippet end
  return EXIT_SUCCESS;
}
