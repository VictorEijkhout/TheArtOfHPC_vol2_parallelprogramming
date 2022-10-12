/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise to illustrate pipelining
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <mpi.h>

// headers just for this exercise
#include <algorithm>
#include <vector>

using namespace std;

#include "tools.h"

#ifndef N
#define N 10
#endif

#ifndef PARTS
#define PARTS 2
#endif

int main(int argc,char **argv) {

  stringstream proctext;

  MPI_Init(&argc,&argv);

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);


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
	proctext << "Can not partition N=" << N << " over PARTS=" << PARTS << endl;
	cerr << proctext.str(); proctext.clear();
	MPI_Abort(comm,1);
      }
    }
  }

  //
  // Exercise:
  // The code here is using blocking sends and receives.
  // Replace by non-blocking.
  //
#if 0
  for (int ipart=0; ipart<PARTS; ipart++) {
    MPI_Recv
      (
       leftdata.data()+partition_starts[ipart],partition_sizes[ipart],
       MPI_DOUBLE,recvfrom,ipart,comm,MPI_STATUS_IGNORE);
    for (int i=partition_starts[ipart];
	 i<partition_starts[ipart]+partition_sizes[ipart];
	 i++)
      myvalue[i] = (procno+1)*(procno+1) + leftdata[i];
    MPI_Send
      (
       myvalue.data()+partition_starts[ipart],partition_sizes[ipart],
       MPI_DOUBLE,sendto,ipart,comm);
  }
#endif
/**** your code here ****/


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
  print_final_result( error_test, comm );

  MPI_Finalize();
  return 0;
}
