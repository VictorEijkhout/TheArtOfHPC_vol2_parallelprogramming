/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% commsplittype.c : illustrating MPI_Comm_split
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
  MPI_Comm_split_type
    (MPI_COMM_WORLD,
     MPI_COMM_TYPE_SHARED,
     procno,info,&sharedcomm);
  MPI_Comm_size
    (sharedcomm,&new_nprocs);
  MPI_Comm_rank
    (sharedcomm,&new_procno);

  ASSERT(new_procno<CORES_PER_NODE);    

  if (new_procno==0) {
    char procname[MPI_MAX_PROCESSOR_NAME]; int namlen;
    MPI_Get_processor_name(procname,&namlen);
    printf("[%d] is processor %d in a shared group of %d\n     running on %s\n",
   procno,new_procno,new_nprocs,procname);
  }
  //if (procno==0) printf("Finished\n");

  MPI_Finalize();
  return 0;
}

