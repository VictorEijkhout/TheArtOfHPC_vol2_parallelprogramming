/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2021-2025
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
  MPI_Comm_set_errhandler(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  stringstream proctext;

#define N 100
  /* const */ int mylocal = N,myfirst{0};
  comm_world.exscan(mpl::plus<int>(),mylocal,myfirst);
  vector<double> indata(mylocal,1.), outdata(mylocal);
  for (int i=0; i<mylocal; i++)
    indata[i] = myfirst+i;
  
  double leftdata=0.,rightdata=0.;
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
		comm_world.isend(indata[mylocal-1],sendto)
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
		comm_world.isend(indata[0],sendto)
/**** your code here ****/
		comm_world.irecv(rightdata,recvfrom)
/**** your code here ****/

  /*
   * Now make sure all Isend/Irecv operations are completed
   */
/**** your code here ****/
  
  /*
   * Do the averaging operation
   * Note that leftdata==rightdata==0 if not explicitly received.
   */
  for (int i=0; i<mylocal; i++)
    if (i==0)
      outdata[i] = leftdata + indata[i] + indata[i+1];
    else if (i==mylocal-1)
      outdata[i] = indata[i-1] + indata[i] + rightdata;
    else
      outdata[i] = indata[i-1] + indata[i] + indata[i+1];
  
  /*
   * Check correctness of the result:
   * value should be 3 times the invalue, except at the end points
   */
  vector<double> answer(N);
  for (int i=0; i<mylocal; i++) {
    answer[i] = 3*(myfirst+i)
      - ( procno==0 && i==0 ) * (-1) // fictitious leftleft elements
      - ( procno==nprocs-1 && i==mylocal-1) * (myfirst+mylocal) // rightright
      ;
    //printf("%e ",answer[i]);
  }
  double error_test = array_error(answer,outdata);
  print_final_result(error_test>1.e-5,comm_world);

  return 0;
}
