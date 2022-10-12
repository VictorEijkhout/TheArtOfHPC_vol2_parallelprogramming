/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

class library {
private:
  MPI_Comm comm;
  int procno,nprocs,other;
  MPI_Request request[2];
public:
  library(MPI_Comm incomm) {
    comm = incomm;
    MPI_Comm_rank(comm,&procno);
    other = 1-procno;
  };
  int communication_start();
  int communication_end();
};

int main(int argc,char **argv) {

#include "globalinit.c"

  int other = 1-procno, sdata=5,rdata;
  MPI_Request request[2];
  MPI_Status status[2];

  if (procno>1) { MPI_Finalize(); return 0; }

  library my_library(comm);
  MPI_Isend(&sdata,1,MPI_INT,other,1,comm,&(request[0]));
  my_library.communication_start();
  MPI_Irecv(&rdata,1,MPI_INT,other,MPI_ANY_TAG,comm,&(request[1]));
  MPI_Waitall(2,request,status);
  my_library.communication_end();

  if (status[1].MPI_TAG==2)
    printf("wrong!\n");

  MPI_Finalize();
  return 0;
}

int library::communication_start() {
  int sdata=6,rdata;
  MPI_Isend(&sdata,1,MPI_INT,other,2,comm,&(request[0]));
  MPI_Irecv(&rdata,1,MPI_INT,other,MPI_ANY_TAG,
	    comm,&(request[1]));
  return 0;
}

int library::communication_end() {
  MPI_Status status[2];
  MPI_Waitall(2,request,status);
  return 0;
}

