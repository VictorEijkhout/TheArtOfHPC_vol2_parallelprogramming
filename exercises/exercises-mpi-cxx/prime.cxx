/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise 
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpi.h>

int main() {
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  int bignum = 2000000111, maxfactor = 45200;
  stringstream proctext;
  
  MPI_Init(0,0);

  // Exercise:
  // -- Parallelize the do loop so that each processor
  //    tries different candidate numbers.
  // -- If a processors finds a factor, print it to the screen.

  // 1. Set loop bounds
/**** your code here ****/
  // 2. Fill in loop header
  for ( int myfactor=
/**** your code here ****/
       ) {
    if (bignum%myfactor==0) {
      proctext << "Processor " << procno << " found factor " << myfactor << endl;
      cerr << proctext.str(); proctext.clear();
    }
  }

  MPI_Finalize();
  return 0;
}
