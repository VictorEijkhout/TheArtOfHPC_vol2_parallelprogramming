/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2018
   %%%%
   %%%% shared.c : illustrating MPI shared memory
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc,char **argv) {
  MPI_Comm comm;
  int nprocs,procid;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procid);

  /*
   * Find the subcommunicator on the node,
   * and get the procid on the node.
   */
  MPI_Comm nodecomm; int onnode_procid;
  MPI_Comm_split_type
    (comm,MPI_COMM_TYPE_SHARED,procid,MPI_INFO_NULL,
     &nodecomm);
  MPI_Comm_rank(nodecomm,&onnode_procid);

  /*
   * Find the subcommunicators of 
   * identical `onnode_procid' processes;
   * the procid on that communicator is the node ID
   */
  MPI_Comm crosscomm; int nodeid;
  MPI_Comm_split
    (comm,onnode_procid,procid,&crosscomm);
  MPI_Comm_rank(crosscomm,&nodeid);
  printf("[%2d] = (%d,%d)\n",procid,nodeid,onnode_procid);

  /*
   * Create data on global process zero,
   * and broadcast it to the zero processes on other nodes
   */
  double shared_data = 0;
  if (procid==0) shared_data = 3.14;
  if (onnode_procid==0)
    MPI_Bcast(&shared_data,1,MPI_DOUBLE,0,crosscomm);
  if (procid==0)
    printf("Head nodes should have shared data: %e\n",
           shared_data);

  /*
   * Create window on the node communicator;
   * it only has nonzero size on the first process
   */
  MPI_Win node_window;
  MPI_Aint window_size; double *window_data;
  if (onnode_procid==0)
    window_size = sizeof(double);
  else window_size = 0;
  MPI_Win_allocate_shared
    ( window_size,sizeof(double),MPI_INFO_NULL,
      nodecomm,
      &window_data,&node_window);

  /*
   * Put data on process zero of the node window
   * We use a Put call rather than a straight copy:
   * the Fence calls enforce coherence
   */
  MPI_Win_fence(0,node_window);
  if (onnode_procid==0) {
    MPI_Aint disp = 0;
    MPI_Put( &shared_data,1,MPI_DOUBLE,0,disp,1,MPI_DOUBLE,node_window);
  }
  MPI_Win_fence(0,node_window);

  /*
   * Now get on each process the address of the window of process zero.
   */
  MPI_Aint window_size0; int window_unit; double *win0_addr;
  MPI_Win_shared_query
    ( node_window,0,
      &window_size0,&window_unit, &win0_addr );

  /*
   * Check that we can indeed get at the data in the shared memory 
   */
  printf("[%d,%d] data at shared window %lx: %e\n",
         nodeid,onnode_procid,(unsigned long)win0_addr,*win0_addr);

  /*
   * cleanup
   */
  MPI_Win_free(&node_window);
  MPI_Finalize();
  return 0;
}
