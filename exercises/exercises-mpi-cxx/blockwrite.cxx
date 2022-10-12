/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-7
   %%%%
   %%%% blockwrite.cxx : MPI IO example
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
   * Exercise: 
   * -- compute an offset (in bytes!) so that processes write
   *    to disjoint parts of the file.
   */
  int nwriters = nprocs-1;
  if (procno<nwriters) {
    MPI_Offset offset =
/**** your code here ****/
      ;
    MPI_File_write_at
      (mpifile,offset,output_data,nwords,MPI_INT,MPI_STATUS_IGNORE);
  }

  MPI_File_close(&mpifile);

  /*
   * Check correctness of the output file
   */
  if (procno==0) {
    int error=nprocs, errors;
    FILE *f;
    f = fopen("blockwrite.dat","r");
    int location = 0;
    proctext << "Trying to read " << nwriters*nwords << " integers\n" << endl;
    cerr << proctext.str(); proctext.clear();
    for (int ip=0; ip<nwriters; ip++) { // loop over processes
      for (int iw=0; iw<nwords; iw++) { // loop over size of local buffer
	int fromfile,success;
	success = fread(&fromfile,sizeof(int),1,f);
	if (success==EOF) {
	  error = procno;
	  proctext << "Premature end of file" << endl;
	  cerr << proctext.str(); proctext.clear();
	  break;
	}
	if (fromfile!=location+1) {
	  error = procno;
	  proctext << "Error " << location << ":" << fromfile << endl;
	  cerr << proctext.str(); proctext.clear();
	}
	location++;
      }
    }
    errors = error;
    fclose(f);
    if (errors==nprocs) 
      proctext << "Execution finished correctly";
    else
      proctext << "Execution finished with errors";
    proctext << endl;
    cerr << proctext.str(); proctext.clear();
  }

  MPI_Finalize();
  return 0;
}

