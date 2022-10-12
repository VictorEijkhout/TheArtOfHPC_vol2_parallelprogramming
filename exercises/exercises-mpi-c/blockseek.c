/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% blockseek.c : MPI IO example with seek
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
  
  MPI_File mpifile;
  /*
   * Exercise:
   * -- open a file for writing;
   *    if it doesn't exist yet, it needs to be created
   */
  MPI_File_open(comm,"blockwrite.dat",
/**** your code here ****/
		MPI_INFO_NULL,
		&mpifile);

  /*
   * Exercise 1: 
   * -- compute an offset (in bytes!) so that processes write
   *    to disjoint parts of the file.
   */
  int nwriters = nprocs-1;
  if (procno<nwriters) {
    MPI_Offset offset =
/**** your code here ****/
      ;
    MPI_File_seek(mpifile,
		  offset,MPI_SEEK_SET);
    /*
     * Loop for writing each word individually
     */
    for (int w=0; w<nwords; w++) {
      MPI_File_write(mpifile,
		     output_data+w,1,MPI_INT,MPI_STATUS_IGNORE);
      /*
       * Exercise 2:
       * -- compute an increment and move the pointer by it
       */
      continue;
      MPI_Offset increment = 
/**** your code here ****/
	;
      MPI_File_seek( mpifile,increment,
/**** your code here ****/
		     );
    }
  } else
    printf("Process %d does not write to file\n",procno);

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
    f = fopen("blockwrite.dat","r");
    int location = 0;
    printf("Trying to read %d integers\n",nwriters*nwords);
    for (int ip=0; ip<nwriters; ip++) { // loop over processes
      for (int iw=0; iw<nwords; iw++) { // loop over size of local buffer
	int fromfile,success;
	success = fread(&fromfile,sizeof(int),1,f);
	if (success==EOF) {
	  error_condition = 1;
	  printf("Premature end of file\n"); break;
	}
	if (fromfile!=location+1) {
	  error_condition = 1;
	  printf("Error loc=%d found=%d expect=%d\n",
		 location,fromfile,location+1);
	}
	location++;
      }
    }
    fclose(f);
  }
  print_final_result(error_condition,comm);

  MPI_Finalize();
  return 0;
}

#if 0
  /*
   * This is working code for 1 process
   */
  if (procno==0) {
    MPI_File_write
      (mpifile,output_data,nwords,MPI_INT,MPI_STATUS_IGNORE);
  }
#endif

