/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% itranspose.c : transposing through non-blocking collectives
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc,char **argv) {

#include "globalinit.c"
  
  double *regular,*transpose;
  regular = (double*) malloc( nprocs*sizeof(double) );
  transpose = (double*) malloc( nprocs*sizeof(double) );
  // each process has column procno
  for (int ip=0; ip<nprocs; ip++)
    regular[ip] = procno*nprocs + ip;

  /*
   * Each proc does a scatter
   */
#if 0
  // reference code:
  for (int iproc=0; iproc<nprocs; iproc++) {
    MPI_Scatter( regular,1,MPI_DOUBLE,
		 &(transpose[iproc]),1,MPI_DOUBLE,
		 iproc,comm);
  }
#endif
  MPI_Request scatter_requests[nprocs];
  for (int iproc=0; iproc<nprocs; iproc++) {
    MPI_Iscatter( regular,1,MPI_DOUBLE,
		  &(transpose[iproc]),1,MPI_DOUBLE,
		  iproc,comm,scatter_requests+iproc);
  }
  MPI_Waitall(nprocs,scatter_requests,MPI_STATUSES_IGNORE);

  /*
   * Check the result
   */
  printf("[%d] :",procno);
  for (int ip=0; ip<nprocs; ip++)
    printf(" %5.2f",transpose[ip]);
  printf("\n");

  MPI_Finalize();

  return 0;
}
