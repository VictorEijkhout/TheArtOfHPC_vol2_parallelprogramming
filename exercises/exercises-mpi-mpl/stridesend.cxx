/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% MPI example for vector type using MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

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

  if (nprocs<2) {
    proctext << "This program needs at least two processes" << endl;
    cerr << proctext.str(); proctext.clear();
    return -1;
  }
  int sender = 0, localsize = 10;

  if (procno==sender) {

    /*
     * Create big data array to be sent
     */
    int ndata = localsize*nprocs;
    int *data = (int*) malloc(ndata*sizeof(int));
    if (!data) {
      proctext << "Out of memory" << endl; 
      cerr << proctext.str(); proctext.clear();
      comm_world.abort(0); }
    for (int i=0; i<ndata; i++)
      data[i] = i;

    /*
     * Exercise
     * - you need a datatype for sending data. Can you define it here....
     */
    int count,stride,blocklength;
/**** your code here ****/

    for (int sendto=0; sendto<nprocs; sendto++) {
      if (sendto==procno)
	continue;
      /*
       * - or do you define the datatype here?
       * - then do a send to the other processor.
       */
/**** your code here ****/
    }
  } else {
    int *mydata = (int*) malloc(localsize*sizeof(int));
    comm_world.recv(mydata,mpl::contiguous_layout<int>(localsize),sender);
    for (int i=0; i<localsize; i++)
      if (mydata[i]%nprocs!=procno) {
	proctext << "[" << procno << "] received element=" << mydata[i]
		 << ", should be " << i*nprocs+procno << endl;
	cerr << proctext.str(); proctext.clear();
      }
  }

  if (procno==0) {
    proctext << "Finished" << endl;
    cerr << proctext.str(); proctext.clear();
  }

  return 0;
}
