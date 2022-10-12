/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-9
   %%%%
   %%%% viewwrite.c : MPI IO example
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpi.h>

int main(int argc,char **argv) {

  MPI_Comm comm;
  int nprocs,procno;
  stringstream proctext;

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
  MPI_File_open(comm,"blockwrite.dat",
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

  if (procno==0) {
    int error=nprocs, errors;
    FILE *f;
    f = fopen("blockwrite.dat","r");
    for (int i=0; i<nwords*nwriters; i++) {
      int fromfile,success;
      success = fread(&fromfile,sizeof(int),1,f);
      if (success==EOF) {
	error = procno;
	proctext << "Premature end of file";
	cerr << proctext.str() << "\n";
	break;
      }
      if (fromfile!=i+1) {
	error = procno;
  	proctext << "Error s/b " << i+1 << ", got " << fromfile;
	cerr << proctext.str() << "\n";
      }
    }
    fclose(f);
    errors = error;
    if (errors==nprocs) 
      proctext << "Execution finished correctly";
    else
      proctext << "Execution finished with errors";
    cerr << proctext.str() << "\n"; 
  }

  MPI_Finalize();
  return 0;
}
