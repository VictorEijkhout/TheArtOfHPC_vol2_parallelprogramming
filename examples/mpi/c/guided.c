/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2018-2023
   %%%%
   %%%% guided.c : splitting guided by hardware
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

  MPI_Info hw_info;
  MPI_Get_hw_resource_info( &hw_info );
  
  MPI_Comm split_comm;
  MPI_Comm_split_type
    ( comm,MPI_COMM_TYPE_HW_GUIDED,procid,MPI_INFO_NULL,&split_comm );

  /*
   * cleanup
   */
  MPI_Finalize();
  return 0;
}
