/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% MPI example for vector type
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <sstream>
#include <vector>
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

  // datatypes, will only be used on the root
  MPI_Datatype scattertype,interleavetype;

  // big data array, will only be created on the root
  vector<int> data;

  if (procno==sender) {
    /*
     * Create big data array to be sent
     */
    int ndata = localsize*nprocs;
    data = vector<int>(ndata);
    for (int i=0; i<ndata; i++)
      data[i] = i;

    /*
     * Exercise
     * - you need a vector datatype for sending data.
     * - resize its extent to achieve proper interleaving
     */
    int count,stride,blocklength;
    MPI_Aint l,e;
    count = localsize; stride = nprocs; blocklength = 1;
    MPI_Type_vector(count,blocklength,stride,MPI_INT,&scattertype);
/**** your code here ****/
  }

  // all processes have local data:
  vector<int> mydata(localsize);
  MPI_Scatter( /* send: */ data.data(),1,interleavetype,
	       /* recv: */ mydata.data(),localsize,MPI_INT,
	       sender,comm
	       );

  for (int i=0; i<localsize; i++)
    if (mydata[i]%nprocs!=procno) {
      proctext << "[" << procno << "] received element=" << mydata[i]
	       << ", should be " << i*nprocs+procno << endl;
      cerr << proctext.str(); proctext.clear();
    }

  if (procno==0) {
    proctext << "Finished" << endl;
    cerr << proctext.str(); proctext.clear();
  }

  if (procno==sender) {
    MPI_Type_free(&interleavetype);
    MPI_Type_free(&scattertype);
  }

  MPI_Finalize();
  return 0;
}
