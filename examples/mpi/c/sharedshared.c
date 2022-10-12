/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-7
   %%%%
   %%%% sharedshared.c : the beginnings of a shared memory 3pt operator
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

#ifndef CORES_PER_NODE
#define CORES_PER_NODE 16
#endif

int main(int argc,char **argv) {

#include "globalinit.c"

  if (nprocs<3) {
    printf("This program needs at least three processes\n");
    return -1;
  }

  if (procno==0)
    printf("There are %d ranks total\n",nprocs);

  int new_procno,new_nprocs;
  MPI_Comm sharedcomm;

  MPI_Info info;
  MPI_Comm_split_type(MPI_COMM_WORLD,MPI_COMM_TYPE_SHARED,procno,info,&sharedcomm);
  MPI_Comm_size(sharedcomm,&new_nprocs);
  MPI_Comm_rank(sharedcomm,&new_procno);

  if (new_nprocs!=nprocs) {
    printf("This example can only run on shared memory\n");
    MPI_Abort(comm,0);
  }
  
  MPI_Win shared_window; int *shared_baseptr;
  MPI_Win_allocate_shared(3,sizeof(int),info,sharedcomm,&shared_baseptr,&shared_window);

  {
    MPI_Aint check_size; int check_unit; int *check_baseptr;
    MPI_Win_shared_query
      (shared_window,new_procno,
       &check_size,&check_unit,&check_baseptr);
    printf("[%d;%d] size=%ld\n",procno,new_procno,check_size);
  }

  int *left_ptr,*right_ptr;
  int left_proc = new_procno>0 ? new_procno-1 : MPI_PROC_NULL,
    right_proc = new_procno<new_nprocs-1 ? new_procno+1 : MPI_PROC_NULL;
  MPI_Win_shared_query(shared_window,left_proc,NULL,NULL,&left_ptr);
  MPI_Win_shared_query(shared_window,right_proc,NULL,NULL,&right_ptr);

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}

