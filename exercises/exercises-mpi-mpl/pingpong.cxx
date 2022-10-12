/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** MPI Exercise using MPL
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <mpi.h>
using namespace std;
#include <mpl/mpl.hpp>

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs, procno;
  stringstream proctext;
  
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

#define NEXPERIMENTS 10000

  // Exercise:
  // -- set source and target processors two ways:
  //    close together and far apart
  // -- run the experiment both ways.
  int processA,processB;
/**** your code here ****/
  double t, send[10000],recv[10000]; send[0] = 1.1;
  if (procno==processA) {
    t = MPI_Wtime();
    for (int n=0; n<NEXPERIMENTS; n++) {
      comm_world.send
	(
/**** your code here ****/
	 );
      comm_world.recv
	(
/**** your code here ****/
	 );
    }
    t = MPI_Wtime()-t; t /= NEXPERIMENTS;
    {
      int nanosec = t*1000000000;
      proctext << "Time for pingpong: "
	       << fixed << setprecision(3) << nanosec*1.e-3 << " (microsec)"
	       << endl;
    }
    cerr << proctext.str(); proctext.clear();
  } else if (procno==processB) {
    for (int n=0; n<NEXPERIMENTS; n++) {
      comm_world.recv
	(
/**** your code here ****/
	 );
      comm_world.send
	(
/**** your code here ****/
	 );
    }
  }

  return 0;
}
