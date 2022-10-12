/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2022
 ****
 **** MPI Exercise for sendrecv : three-point combination
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpi.h>

#include "tools.h"

int main() {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  stringstream proctext;
  
  MPI_Init(0,0);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  double mydata=procno, leftdata=0, rightdata=0;
  int sendto,recvfrom;

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  // -- once to get data from the left, once from the right

  // first get left neighbor data
  //hint:  sendto = 
  //hint:  recvfrom =
/**** your code here ****/
  MPI_Sendrecv
    (&mydata,1,MPI_DOUBLE, sendto,0,
     &leftdata,1,MPI_DOUBLE, recvfrom,0, comm,MPI_STATUS_IGNORE);

  // then the right neighbor data
  //hint:  recvfrom =
  //hint:  sendto = 
/**** your code here ****/
  MPI_Sendrecv
    (&mydata,1,MPI_DOUBLE, sendto,0,
     &rightdata,1,MPI_DOUBLE, recvfrom,0, comm,MPI_STATUS_IGNORE);

  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  mydata = mydata+leftdata+rightdata;

  double res;
  if (procno==0) {
    res = 2*procno+1;
  } else if (procno==nprocs-1) {
    res = 2*procno-1;
  } else {
    res = 3*procno;
  }

  int error_test = !ISAPPROX(mydata,res);
  if (error_test) {
    stringstream proctext;
    proctext << "Data on proc " << procno << " should be " << res << ", found " << mydata;
    cout << proctext.str() << "\n";
  }
  print_final_result(error_test,comm);

  MPI_Finalize();
  return 0;
}
