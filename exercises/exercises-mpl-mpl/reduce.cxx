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
#include <random>
#include <mpl/mpl.hpp>

float random_float() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(0.0, 1.0);
  return dis(gen);
}

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  
  int nprocs, procno;
  nprocs = comm_world.size();
  procno = comm_world.rank();
  //solved

  //
  // Every process goes its own random number
  //
  float myrandom = random_float();
  //  std::cout << myrandom << '\n';

  // Exercise part 1:
  // - sum all the random values together
  // - each process divides their number by this sum
  // - for bonus points: check that the normalized values sum up to 1
  float sum_of_randoms;
/**** your code here ****/
  float scaled_random = myrandom / sum_of_randoms;
  std::cout << std::format( "{:2} scaled {:.4} -> {:.4}",procno,myrandom,scaled_random ) << '\n';

  // Exercise part 2:
  // only the root processs prints out the maximum random value
  // Use two different overloads of the reduce function
  int root = nprocs-1;
/**** your code here ****/
  
  return 0;
}
