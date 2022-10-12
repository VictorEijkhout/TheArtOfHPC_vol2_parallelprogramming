/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% itransposeblock.c : transposing through non-blocking collectives
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
  
  /*
   * Allocate matrix and transpose:
   * - one column per rank for regular
   * - one row per rank for transpose
   */
  double *regular,*transpose;
  regular = (double*) malloc( nprocs*sizeof(double) );
  transpose = (double*) malloc( nprocs*sizeof(double) );
  // each process has columnns m*nprocs -- m*(nprocs+1)
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
#else
  MPI_Request scatter_requests[nprocs];
  for (int iproc=0; iproc<nprocs; iproc++) {
    MPI_Iscatter( regular,1,MPI_DOUBLE,
		  &(transpose[iproc]),1,MPI_DOUBLE,
		  iproc,comm,scatter_requests+iproc);
  }
  MPI_Waitall(nprocs,scatter_requests,MPI_STATUSES_IGNORE);
#endif

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
