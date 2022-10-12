/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2020
 ****
 **** MPI Exercise using MPL
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <random>
using namespace std;
#include <mpl/mpl.hpp>

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs, procno;
  
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();
  
  // set the default random number generator
  std::default_random_engine generator;
  // distribution: real between 0 and 1
  std::uniform_real_distribution<float> distribution(0.,1.);

  // Compute a normalized random number and print for good measure
  float
    myrandom,globalrandom;
  myrandom = distribution(generator);
  for (int p=0; p<procno; p++) {
    // make sure we have p independent values
    distribution.reset(); myrandom = distribution(generator);
  }
  {
    stringstream proctext;
    proctext << "Process " << setw(3) << procno
	     << " has random value " << fixed << setprecision(10) << myrandom
	     << endl;
    cerr << proctext.str(); proctext.clear();
  }

  /*
   * Exercise part 1:
   * -- compute the sum of the values, everywhere
   * -- scale your number by the sum
   * -- check that the sum of scales values is 1
   */
  float sumrandom, scaled_random, sum_scaled_random;
  comm_world.allreduce
    (
/**** your code here ****/
     );
  scaled_random = myrandom / sumrandom;
  comm_world.allreduce
    (
/**** your code here ****/
     );

  /*
   * Correctness test
   */
  int error=nprocs, errors;
  if ( abs(sum_scaled_random-1.)>1.e-5 ) {
    stringstream proctext;
    proctext << "Suspicious sum " << sumrandom << " on process " << procno << endl;
    cerr << proctext.str();
    error = procno;
  }
  comm_world.reduce(mpl::min<int>(),0,error,errors);
  if (procno==0) {
    stringstream proctext;
    if (errors==nprocs) 
      proctext << "Part 1 finished; all results correct" << endl;
    else
      proctext << "Part 1: first error occurred on rank " << errors << endl;
    cerr << proctext.str();
  }

#if 0
  // Exercise part 2:
  // -- compute the maximum random value on process zero
  comm_world.reduce
    (
/**** your code here ****/
     );
  if (procno==0) {
    stringstream proctext;
    proctext << "Part 2: The maximum number is " << globalrandom << endl;
    cerr << proctext.str();
  }
#endif

  return 0;
}
