/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2018/9
   %%%%
   %%%% shared.c : illustrating MPI shared memory
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;
#include "mpi.h"

int main(int argc,char **argv) {
  stringstream proctext;
  MPI_Comm comm;
  int nprocs,procno;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  /*
   * Exercise:
   * -- construct `nodecomm': the communicator that contains all processes on this node.
   *    the correct type is in the book / slides
   * Find the subcommunicator on the node,
   * and get the procno on the node.
   */
  MPI_Comm nodecomm; int onnode_procno;
  MPI_Comm_split_type
    (comm,
/**** your code here ****/
     procno,
     MPI_INFO_NULL,
     &nodecomm);
  /*
   * Exercise:
   * -- compute the rank on the node
   */
  MPI_Comm_rank
    (
/**** your code here ****/
     &onnode_procno);

  /*
   * Exercise:
   * -- make subcommunicators of
   *    identical `onnode_procno' processes;
   *    the procno on that communicator is the node ID
   */
  MPI_Comm crosscomm; int nodeid;
  MPI_Comm_split
    (
/**** your code here ****/
     procno,&crosscomm);
  MPI_Comm_rank(crosscomm,&nodeid);
  proctext << "[" << procno << "] has node,onnode coordinate "
	   << nodeid << "," << onnode_procno << endl;
  cerr << proctext.str(); proctext.clear();
    

  /*
   * Create data on global process zero,
   * and broadcast it to the zero processes on other nodes
   */
#define REFVAL 3.14
  double shared_data = 0;
  if (procno==0) shared_data = REFVAL;
  if (onnode_procno==0) {
    MPI_Bcast(&shared_data,1,MPI_DOUBLE,0,crosscomm);
    proctext << "[" << procno << "] Head nodes should have shared data: "
	     << shared_data << endl;
    cerr << proctext.str(); proctext.clear();    
  }

  /*
   * Exercise:
   * -- Create window on the node communicator;
   *    it only has nonzero size on the first process
   * -- What happens if you use MPI_Win_allocate ?
   */
  MPI_Aint window_size; double *window_data; MPI_Win node_window;
  if (onnode_procno==0)
    window_size = sizeof(double);
  else window_size = 0;
  MPI_Win_allocate_shared( window_size,sizeof(double),MPI_INFO_NULL,
/**** your code here ****/
			   &window_data,&node_window);

  /*
   * Put data on process zero of the node window
   * We use a Put call rather than a straight copy:
   * the Fence calls enforce coherence
   */
  MPI_Win_fence(0,node_window);
  if (onnode_procno==0) {
    MPI_Aint disp = 0;
    MPI_Put( &shared_data,1,MPI_DOUBLE,0,disp,1,MPI_DOUBLE,node_window);
  }
  MPI_Win_fence(0,node_window);

  /*
   * Now get on all processes the address of the window of process zero.
   */
  MPI_Aint window_size0; int window_unit; double *win0_addr;
  MPI_Win_shared_query( node_window,0,
			&window_size0,&window_unit, &win0_addr );

  /*
   * Check that we can indeed get at the data in the shared memory 
   */
  //printf("[%d,%d] data at shared window: %e\n",nodeid,onnode_procno,*win0_addr);
  double check_val = *win0_addr;
  int
    error = fabs(check_val-REFVAL)/REFVAL > 1.e-14 ? procno : nprocs,
    errors=-1;
  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct\n";
    else
      proctext << "First error occurred on proc " << errors << "\n";
  }
  cerr << proctext.str(); proctext.clear();

  /*
   * cleanup
   */
  MPI_Win_free(&node_window);
  MPI_Finalize();
  return 0;
}
