/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-7
   %%%%
   %%%% viewwrite.c : MPI IO example
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

#include "tools.h"

int main(int argc,char **argv) {

  MPI_Comm comm;
  int nprocs,procno;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  /*
   * Each process has a buffer of length `nwords'
   * containing consecutive integers
   */
  int nwords = 3;
  int *output_data = (int*) malloc(nwords*sizeof(int));
  for (int iw=0; iw<nwords; iw++)
    output_data[iw] = procno*nwords+iw+1;
  
  /*
   * Create the output file and open for write
   */
  MPI_File mpifile;
  MPI_File_open(comm,"viewwrite.dat",
		MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,
		&mpifile);

  /*
   * Exercise:
   * -- set a file view so that each processor writes at a different offset
   * -- Note: offsets are in bytes
   */
  MPI_File_set_view
    (mpifile,
/**** your code here ****/
     MPI_INT,"native",MPI_INFO_NULL);

  int nwriters = 2;
  if (procno<nwriters) {
    MPI_File_write // no offset, because that is taken care of by the view
      (mpifile,output_data,nwords,MPI_INT,MPI_STATUS_IGNORE);
  }

  MPI_File_close(&mpifile);

  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  int error_condition = 0;
  if (procno==0) {
    FILE *f;
    f = fopen("viewwrite.dat","r");
    for (int i=0; i<nwords*nwriters; i++) {
      int fromfile,success;
      success = fread(&fromfile,sizeof(int),1,f);
      if (success==EOF) {
  	printf("Premature end of file\n");
	error_condition = 1;
	break;
      }
      if (fromfile!=i+1) {
  	printf("Error s/b %d, got %d\n",i+1,fromfile);
	error_condition = 1;
	break;
      }
    }
    fclose(f);
  }
  print_final_result(error_condition,comm);

  MPI_Finalize();
  return 0;
}
