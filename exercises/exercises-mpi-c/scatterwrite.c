/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-7
   %%%%
   %%%% scatterwrite.c : MPI IO example
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

  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  /*
   * Each process has a buffer of length `nwords'
   * containing consecutive integers
   */
  int nwords = 3;
  int *output_data = (int*) malloc(nwords*sizeof(int));
  for (int iw=0; iw<nwords; iw++)
    output_data[iw] = procno*nwords + iw+1;
  
  MPI_File mpifile;
  MPI_File_open(comm,"scatterwrite.dat",
		MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,
		&mpifile);

  /*
   * Exercise:
   * -- set a file view so that each processor writes at a different offset
   * -- Note: offsets are in bytes
   */
  MPI_Datatype scattertype;
  MPI_Type_vector(
/**** your code here ****/
		  MPI_INT,&scattertype);
  MPI_Type_commit(&scattertype);
  MPI_Offset offset = 
/**** your code here ****/
    ;
  MPI_File_set_view
    (mpifile,
     offset,MPI_INT,scattertype,
     "native",MPI_INFO_NULL);

  int nwriters = nprocs;
  if (procno<nwriters) {
    MPI_File_write // no offset, because that is taken care of by the view
      (mpifile,output_data,nwords,MPI_INT,MPI_STATUS_IGNORE);
  }

  MPI_Type_free(&scattertype);
  MPI_File_close(&mpifile);

  /*
   * Check correctness
   */
  if (procno==0) {
    FILE *f;
    f = fopen("scatterwrite.dat","r");
    int location = 0;
    for (int iw=0; iw<nwords; iw++) { // loop over size of local buffer
      for (int ip=0; ip<nwriters; ip++) { // loop over processes
	int fromfile,success;
	success = fread(&fromfile,sizeof(int),1,f);
	if (success==EOF) {
	  printf("Premature end of file\n"); break;
	}
	int itest = iw+1 + nwords*ip;
	if (fromfile!=itest) {
	  printf("Error   %d:%d:%d\n",location,itest,fromfile);
	} // else printf("Correct %d:%d:%d\n",location,itest,fromfile);
	location++;
      }
    }
    fclose(f);
    printf("Finished; all tasks correctly concluded.\n");
  }

  MPI_Finalize();
  return 0;
}
