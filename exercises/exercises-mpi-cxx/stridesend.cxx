/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-9
   %%%%
   %%%% MPI example for vector type
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <sstream>
using namespace std;
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm; int nprocs,procno;
  stringstream proctext;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  if (nprocs<2) {
    proctext << "This program needs at least two processes" << endl;
    cerr << proctext.str(); proctext.clear();
    return -1;
  }
  int sender = 0, localsize = 10;

  if (procno==sender) {
    /*
     * Create big data array to be sent
     */
    int ndata = localsize*nprocs;
    int *data = (int*) malloc(ndata*sizeof(int));
    if (!data) {
      proctext << "Out of memory" << endl; 
      cerr << proctext.str(); proctext.clear();
      MPI_Abort(comm,0); }
    for (int i=0; i<ndata; i++)
      data[i] = i;
    /*
     * Exercise
     * - you need a datatype for sending data. Can you define it here....
     */
    MPI_Datatype scattertype;
    int count,stride,blocklength;
/**** your code here ****/
    for (int sendto=0; sendto<nprocs; sendto++) {
      if (sendto==procno)
	continue;
      /*
       * - or do you define the datatype here?
       * - then do a send to the other processor.
       */
/**** your code here ****/
    }
/**** your code here ****/
  } else {
    int *mydata = (int*) malloc(localsize*sizeof(int));
    MPI_Recv(mydata,localsize,MPI_INT,sender,0,comm,MPI_STATUS_IGNORE);
    for (int i=0; i<localsize; i++)
      if (mydata[i]%nprocs!=procno) {
	proctext << "[" << procno << "] received element=" << mydata[i]
		 << ", should be " << i*nprocs+procno << endl;
	cerr << proctext.str(); proctext.clear();
      }
  }

  if (procno==0) {
    proctext << "Finished" << endl;
    cerr << proctext.str(); proctext.clear();
  }

  MPI_Finalize();
  return 0;
}
