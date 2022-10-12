/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** MPI Exercise for the subarray type, MPL version
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <random>
using namespace std;
#include <mpl/mpl.hpp>

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs, procno;
  
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  stringstream proctext;
  
  /*
   * See if you can arrange the processes
   * in a perfect cube. If not, quit.
   */
  int procs_per_side;
  for (procs_per_side=1; ; procs_per_side++) {
    if (procs_per_side*procs_per_side*procs_per_side==nprocs)
      break;
    if (procs_per_side*procs_per_side*procs_per_side>nprocs) {
      if (procno==0) {
	proctext << "Number of processes needs to be a cube" << endl;
	cerr << proctext.str(); proctext.clear();
      }
      comm_world.abort(0); //      MPI_Abort(comm,0);
    }
  }

  /*
   * Every process gets its own data and sends it to process zero.
   * Exercise: why are we using an Isend here?
   */
  int mydata = procno+1;
  //  MPI_Request send_request;
  auto send_request = comm_world.isend(mydata,0);
  
  /*
   * Now process zero receives all the contribution using the subarray type.
   * (You could use a gather here, but only because it is just one element)
   */
  if (procno==0) {
    vector<int> cubedata(procs_per_side*procs_per_side*procs_per_side);
    int sides[3], sub_sizes[3], startpoint[3];
    for (int proci=0; proci<procs_per_side; proci++) {
      for (int procj=0; procj<procs_per_side; procj++) {
	for (int prock=0; prock<procs_per_side; prock++) {
	  /*
	   * Now receive the data, using a subarray type to
	   * put it in the right location of the cubedata buffer.
	   */
/**** your code here ****/
	  //MPI_Datatype insert_type;
	  mpl::subarray_layout<int> insert_type
	    ( { 
	      { procs_per_side,1,proci },
	      { procs_per_side,1,procj },
	      { procs_per_side,1,prock }
	    } );
	  /*
	   * Now do the receive call
	   */
	  int sender = ( proci*procs_per_side + procj )*procs_per_side + prock;
	  comm_world.recv
	    (
/**** your code here ****/
	     sender);
	}
      }
    }

    proctext << "Received cube:";
    int error{0};
    for (int p=0; p<nprocs; p++) {
      if (cubedata[p]!=p+1)
	error++;
      proctext << " " << cubedata[p];
    }
    proctext << "\n";
    if (error==0)
      proctext << "Finished. Cube correctly gathered.";
    else
      proctext << "There were errors";
    proctext << endl;
    cerr << proctext.str(); 
  }
  send_request.wait();
  
  return 0;
}
