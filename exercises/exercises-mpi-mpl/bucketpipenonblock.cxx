/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** MPI Exercise to illustrate pipelining in MPL
 ****
 ****************************************************************/

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <vector>
using namespace std;
#include <mpl/mpl.hpp>

#include "tools.h"

#ifndef PARTS
#define PARTS 2
#endif

#ifndef N
#define N 1000
#endif

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  // Set `sendto' and `recvfrom'
  int sendto =
    ( procno<nprocs-1 ? procno+1 : MPI_PROC_NULL )
    ;
  int recvfrom =
    ( procno>0 ? procno-1 : MPI_PROC_NULL )
    ;

  // Set up data array
  vector<double> leftdata(N,0.), myvalue(N,0.);

  // Set up blocking for the pipeline
  int partition_starts[PARTS], partition_sizes[PARTS];
  { int points_left=N, block = N/PARTS;
    for (int ipart=0; ipart<PARTS; ipart++) {
      partition_starts[ipart] = ipart*block;
      if (ipart<PARTS-1)
	partition_sizes[ipart] = block;
      else
	partition_sizes[ipart] = points_left;
      points_left -= partition_sizes[ipart];
      if (points_left<0) {
	stringstream proctext;
	proctext << "Can not partition N=" << N << " over PARTS=" << PARTS << endl;
	cerr << proctext.str();
	comm_world.abort(1);
      }
    }
  }

  //
  // Exercise:
  // The code here is using blocking sends and receives.
  // Replace by non-blocking.
  //
#if 1
  for (int ipart=0; ipart<PARTS; ipart++) {
    comm_world.recv
      (
       leftdata.data()+partition_starts[ipart],
       mpl::contiguous_layout<double>(partition_sizes[ipart]),
       recvfrom,mpl::tag(ipart)
       );
    for (int i=partition_starts[ipart];
	 i<partition_starts[ipart]+partition_sizes[ipart];
	 i++)
      myvalue[i] = (procno+1)*(procno+1) + leftdata[i];
    comm_world.send
      (
       myvalue.data()+partition_starts[ipart],
       mpl::contiguous_layout<double>(partition_sizes[ipart]),
       sendto,mpl::tag(ipart)
       );
  }
#else
/**** your code here ****/
#endif


  /*
   * Check correctness
   */
  double p1 = procno+1.;
  double my_sum_of_squares = p1*p1*p1/3 + p1*p1/2 + p1/6;
  vector<double> correct(N,my_sum_of_squares);
  double relative_error = array_error(myvalue,correct); //relative_error > 1.e-12;
  int error_test = relative_error>1.e-12;
  if (error_test) {
    stringstream proctext;
    proctext << "Relative error on proc " << procno << " : " << relative_error;
    cout << proctext.str() << "\n";
  }
  print_final_result( error_test, comm_world );

  return 0;
}
