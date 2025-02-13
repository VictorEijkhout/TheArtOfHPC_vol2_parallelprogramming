/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2025
   %%%%
   %%%% MPI example for vector type and extent resizing in MPL
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
  vector<int> mydata(localsize);

  if (procno==sender) {

    /*
     * Create big data array to be sent
     */
    int ndata = localsize*nprocs;
    vector<int> data(ndata);
    for (int i=0; i<ndata; i++)
      data[i] = i;

    /*
     * Exercise
     * - you need a vector datatype for sending data.
     * - resize its extent to achieve proper interleaving
     */
    int count,stride,blocklength;
    count = localsize; stride = nprocs; blocklength = 1;
/**** your code here ****/

    comm_world.scatter
      ( sender,
	data.data(),scattertype,
	mydata.data(),mpl::contiguous_layout<int>(localsize) );
  } else {
    comm_world.scatter
      ( sender,
	mydata.data(),mpl::contiguous_layout<int>(localsize) );
  }


#if 0
  /*
   * Trace output is disabled by default;
   * change the 0 to 1 to get a full printout
   * of the data of each process
   */
  printf("[%2d]",procno);
  for (int i=0; i<localsize; i++)
    printf(" %2d",mydata[i]);
  printf("\n");
#endif
  
  /*
   * Correctness checking
   */
  for (int i=0; i<localsize; i++)
    if (mydata[i]%nprocs!=procno)
      printf("[%d] received element=%d, should be %d\n",procno,mydata[i],i*nprocs+procno);

  if (procno==0)
    printf("Finished\n");

  return 0;
}
