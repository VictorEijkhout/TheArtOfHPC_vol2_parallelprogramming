/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2023
   %%%%
   %%%% inttag.cxx : integer tags
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <iostream>
#include <mpl/mpl.hpp>
 
int main() {
  const mpl::communicator &
    world(mpl::environment::comm_world());
  if (world.size()<2)
    world.abort(EXIT_FAILURE);
  double x[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  //codesnippet mplinttag
  enum class pingpongtag : int { ping=1, pong=2 };
  int pinger = 0, ponger = world.size()-1;
  if (world.rank()==pinger) {
    world.send(x, ponger, pingpongtag::ping);
    world.recv(x, ponger, pingpongtag::pong);
  } else if (world.rank()==ponger) {
    world.recv(x, pinger, pingpongtag::ping);
    world.send(x, pinger, pingpongtag::pong);
  }
  //codesnippet end
  if (world.rank()==pinger or world.rank()==ponger)
    std::cout << "pingpong concluded\n";

  return EXIT_SUCCESS;
}
