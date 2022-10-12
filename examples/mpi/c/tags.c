/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% tags.c : getting tgs upper bound
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv) {

#ifndef FREQUENCY
#define FREQUENCY -1
#endif

  /*
   * Standard initialization
   */
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procid;
  MPI_Init(&argc,&argv);
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procid);
  int ierr;

  if (nprocs<2) {
    printf("This test needs at least 2 processes, not %d\n",nprocs);
    MPI_Abort(comm,0);
  }
  int sender = 0, receiver = nprocs-1;
  if (procid==0) {
    printf("Running on comm world of %d procs; communicating between %d--%d\n",
	   nprocs,sender,receiver);
  }

  int tag_upperbound;
  void *v; int flag=1;
  ierr = MPI_Comm_get_attr(comm,MPI_TAG_UB,&v,&flag);
  tag_upperbound = *(int*)v;
  if (ierr!=MPI_SUCCESS) {
    printf("Error getting attribute: return code=%d\n",ierr);
    if (ierr==MPI_ERR_COMM)
      printf("invalid communicator\n");
    if (ierr==MPI_ERR_KEYVAL)
      printf("errorneous keyval\n");
    MPI_Abort(comm,0);
  }
  if (!flag) {
    printf("Could not get keyval\n");
    MPI_Abort(comm,0);
  } else {
    if (procid==sender)
      printf("Determined tag upperbound: %d\n",tag_upperbound);
  }


  MPI_Finalize();
  return 0;
}
