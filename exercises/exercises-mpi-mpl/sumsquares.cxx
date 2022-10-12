/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** sumsquares.cxx
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs, procno;
  stringstream proctext;
  
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  /*
   * You can call this program as "sumsquare 12345"
   * where the numeric argument is the number of elements
   * on each processor. There is a default if you don't
   * specify this.
   */
  int nlocal,nglobal;
  if (procno==0) {
    if (argc>1)
      nlocal = atoi(argv[1]);
    else nlocal = 1000;
  }
  comm_world.bcast(0,nlocal);
  nglobal = nprocs*nlocal;

  // Allocate local data. We use doubles rather than integers (why?)
  vector<double> local_squares(nlocal);

  /*
   * Exercise part 1:
   * -- set your local values
   */

  for (int i=0; i<nlocal; i++) {
/**** your code here ****/
  }

  /*
   * Exercise part 2:
   * -- do the local summation
   * -- do the global summation
   */
  double local_sum = 0., global_sum;
  for (int i=0; i<nlocal; i++) {
/**** your code here ****/
  }
  comm_world.reduce
    (
/**** your code here ****/
     );
  if (procno==0) {
    stringstream proctext;
    double fglobal = nglobal;
    proctext << "Global sum: " << global_sum
	     << " should be "
	     << (1./3.)*fglobal*fglobal*fglobal + (1./2.)*fglobal*fglobal + fglobal/6.;
    cerr << proctext.str() << "\n";
  }

  return 0;
}
