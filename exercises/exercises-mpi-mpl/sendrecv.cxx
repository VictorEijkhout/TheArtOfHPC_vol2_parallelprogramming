/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2022
 ****
 **** MPI Exercise for sendrecv : three-point combination in MPL
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

#include "tools.h"

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  stringstream proctext;
  
  double mydata=1,leftdata=0,rightdata=0;
  int sendto,recvfrom;

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  // -- once to get data from the left, once from the right

  // first get left neighbor data
  //hint:  sendto = 
  //hint:  recvfrom =
/**** your code here ****/
  mpl::tag t0(0);
  comm_world.sendrecv
    ( mydata,sendto,t0,
      leftdata,recvfrom,t0 );

  // then the right neighbor data
  //hint:  recvfrom =
  //hint:  sendto = 
/**** your code here ****/
  comm_world.sendrecv( mydata,sendto,t0, rightdata,recvfrom,t0 );

  // check correctness
  mydata = mydata+leftdata+rightdata;

  double res;
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
