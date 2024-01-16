/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2023
 ****
 **** MPI Exercise using MPL layouts on a pingpong
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

#define NEXPERIMENTS 100
#ifndef SIZE
#define SIZE 10000
#endif

  // Exercise:
  // -- define a layout for the buffer
  // -- use the layout to send the buffer
  int processA,processB;
  processA = 0; processB = nprocs-1;
  double t, send[SIZE],recv[SIZE]; send[0] = 1.1;
  // use mpl::contiguous_layout
/**** your code here ****/
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
      proctext << "Time for pingpong of size " << SIZE << ": "
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
