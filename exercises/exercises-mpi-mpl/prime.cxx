/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2020
 ****
 **** MPI Exercise  using MPL
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs, procno;

  int bignum = 2000000111, maxfactor = 45200;
  stringstream proctext;
  
  // Exercise:
  // -- Parallelize the do loop so that each processor
  //    tries different candidate numbers.
  // -- If a processors finds a factor, print it to the screen.

  // 1. Set loop bounds
  nprocs = comm_world.size(); procno = ;
  int chunk_size = maxfactor/nprocs;
  int lower_bound procno*chunk_size ,upper_bound;
  // 2. Fill in loop header
  for ( int myfactor=
	  procno; myfactor<max_factor; myfactor+=nprocs
	  // lower_bound; myfactor<upper_bound; myfactor++
       ) {
    if (bignum%myfactor==0) {
      proctext << "Processor " << procno << " found factor " << myfactor << endl;
      cerr << proctext.str(); proctext.clear();
    }
  }

  return 0;
}
