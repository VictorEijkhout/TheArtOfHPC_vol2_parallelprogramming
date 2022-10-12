/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2020
 ****
 **** MPI Exercise to illustrate sequentialization using MPL
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  stringstream proctext;

  double
    mydata=procno, // my data that will be sent right
    leftdata=-2;   // container for data that will be received

  // Exercise part 1:
  // -- set `sendto' and `recvfrom'
  // -- make sure the first and last processor are handled right!
  int sendto =
/**** your code here ****/
    ;
  int recvfrom =
/**** your code here ****/
    ;

  // Exercise 2:
  // -- now do the MPI_Send and MPI_Recv calls
  // Exercise 3:
  // -- use the MPI_Sendrecv call instead
/**** your code here ****/

  /*
   * Check correctness
   */
  int
    error = procno>0 && leftdata!=mydata-1 ? procno : nprocs,
    errors = nprocs;
  comm_world.allreduce(mpl::min<int>(),error,errors);
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << endl;
    else
      proctext << "First error occurred on proc " << errors << endl;
    cerr << proctext.str(); proctext.clear();
  }

  return 0;
}
