/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2020
 ****
 **** MPI Exercise for Isend/Irecv using MPL
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

#include "tools.h"

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  stringstream proctext;

  int mydata=1.,leftdata=0.,rightdata=0.;
  int sendto,recvfrom;
  mpl::irequest_pool requests;

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  //    once to get data from the left, once from the right
  // -- for first/last process use MPI_PROC_NULL

  /*
   * Stage 1: get data from the left.
   * Exercise: who are you communicating with?
   */
/**** your code here ****/
  /*  
   * Start isend and store the request
   */
/**** your code here ****/
		comm_world.isend(mydata,sendto)
/**** your code here ****/
		comm_world.irecv(leftdata,recvfrom)
/**** your code here ****/


  /*
   * Stage 2: data from the right
   * Exercise: who are you communicating with?
   */
/**** your code here ****/
  /* 
   * Start isend and store the request
   */
/**** your code here ****/
		comm_world.isend(mydata,sendto)
/**** your code here ****/
		comm_world.irecv(rightdata,recvfrom)
/**** your code here ****/

  /*
   * Now make sure all Isend/Irecv operations are completed
   */
/**** your code here ****/
  
  // check correctness
  mydata = mydata+leftdata+rightdata;

  int res;
  if (procno==0) {
    res = 2*procno+1;
  } else if (procno==nprocs-1) {
    res = 2*procno-1;
  } else {
    res = 3*procno;
  }

  int error_test = !isapprox(mydata,res);
  if (error_test) {
    stringstream proctext;
    proctext << "Data on proc " << procno << " should be " << res << ", found " << mydata;
    cout << proctext.str() << "\n";
  }
  print_final_result(error_test,comm_world);

  return 0;
}
