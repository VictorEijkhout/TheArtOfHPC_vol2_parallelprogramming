/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2019
   %%%%
   %%%% numa.c : investigating NUMA effects in MPI shared memory
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <mpi.h>
#include "window.c"

int main(int argc,char **argv) {

#include "globalinit.c"

  /*
   * Find the subcommunicator on the node,
   * and get the procid on the node.
   */
  MPI_Comm nodecomm;
  int onnode_procno, onnode_nprocs;
  MPI_Comm_split_type
    (comm,MPI_COMM_TYPE_SHARED,procno,MPI_INFO_NULL,
     &nodecomm);
  MPI_Comm_size(nodecomm,&onnode_nprocs);
  /* if (onnode_nprocs<2) { */
  /*   printf("This example needs at least two ranks per node\n"); */
  /*   //    MPI_Abort(comm,0); */
  /* } */
  MPI_Comm_rank(nodecomm,&onnode_procno);

  for (int strategy=0; strategy<2; strategy++) {
    /*
     * Create window on the node communicator;
     * one item on each process
     */
    MPI_Aint window_size; double *window_data; MPI_Win node_window;
    window_size = sizeof(double);
    MPI_Info window_info;
    MPI_Info_create(&window_info);
    if (strategy==0) {
      if (procno==0)
        printf("Strategy 0 : default behavior of shared window allocation\n");
      MPI_Info_set(window_info,"alloc_shared_noncontig","false");
    } else {
      if (procno==0)
        printf("Strategy 1 : allow non-contiguous shared window allocation\n");
      MPI_Info_set(window_info,"alloc_shared_noncontig","true");
    }
    MPI_Win_allocate_shared( window_size,sizeof(double),window_info,
                             nodecomm,
                             &window_data,&node_window);
    MPI_Info_free(&window_info);
    test_window(node_window,nodecomm);

    /*
     * Now process zero checks on window placement
     */
    if (onnode_procno==0) {
      MPI_Aint window_size0; int window0_unit; double *win0_addr;
      MPI_Win_shared_query( node_window,0,
                            &window_size0,&window0_unit, &win0_addr );
      size_t dist1,distp;
      for (int p=1; p<onnode_nprocs; p++) {
        MPI_Aint window_sizep; int windowp_unit; double *winp_addr;
        MPI_Win_shared_query( node_window,p,
                              &window_sizep,&windowp_unit, &winp_addr );
        distp = (size_t)winp_addr-(size_t)win0_addr;
        if (procno==0)
          printf("Distance %d to zero: %ld\n",p,(long)distp);
        if (p==1)
          dist1 = distp;
        else {
          if (distp%dist1!=0)
            printf("!!!! not a multiple of distance 0--1 !!!!\n");
        }
      }
    }
    MPI_Win_free(&node_window);
  }

  /*
   * cleanup
   */
  MPI_Finalize();
  return 0;
}
