/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-8
   %%%%
   %%%% env.c : investigate the INFO_ENV object
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  int procno,nprocs;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);

  if (procno==0) {
    char info_string[100]; int flag;

    MPI_Info_get( MPI_INFO_ENV,"command",100,info_string,&flag);
    if (!flag)
      printf("Info command could not be found\n");
    else
      printf("Info command: <<%s>>\n",info_string);

    MPI_Info_get( MPI_INFO_ENV,"maxprocs",100,info_string,&flag);
    if (!flag)
      printf("Info maxprocs could not be found\n");
    else
      printf("Info maxprocs: <<%s>>\n",info_string);

    MPI_Info_get( MPI_INFO_ENV,"arch",100,info_string,&flag);
    if (!flag)
      printf("Info arch could not be found\n");
    else
      printf("Info arch: <<%s>>\n",info_string);
  }

  MPI_Finalize();
  return 0;
}
