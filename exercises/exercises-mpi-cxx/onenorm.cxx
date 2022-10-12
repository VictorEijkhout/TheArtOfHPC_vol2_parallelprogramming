/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2019
   %%%%
   %%%% onenorm.c : user-defined one-norm
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;
#include <mpi.h>

void one_norm(void *in,void *inout,int *len,MPI_Datatype *type) {
  // r is the already reduced value, n is the new value
  float n = *(float*)in, r = *(float*)inout;
  float m;

  /*
   * Exercise:
   * -- write the correct reduction step 
   *    for computing a 1-norm
   */
/**** your code here ****/
#ifdef DEBUG
  stringstream proctext;
  proctext << "combine " << r << " " << n << ": " << m << "\n";
  cerr << proctext.str();
#endif
  *(float*)inout = m;  
}

int main(int argc,char **argv) {

  MPI_Comm comm;
  int procno=-1,nprocs,err;
  MPI_Init(&argc,&argv); 
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno); 
  MPI_Comm_size(comm,&nprocs); 

  /* 
   * Data specific for this program:
   * every process stores one number, minus-one,
   * which makes the one-norm equal to the 
   * number of processes.
   */  
  float data,data_one_norm;
  data = -1.e0;

  /*
   * Create the operator,
   * do the reduction,
   * free the operator.
   */
  MPI_Op calc_one;
  MPI_Op_create(one_norm,1,&calc_one);
  MPI_Allreduce(&data,&data_one_norm,1,MPI_FLOAT,calc_one,comm);
  MPI_Op_free(&calc_one);

  // check that the distributed result is the same as sequential
  float actual_norm = nprocs;
  stringstream proctext;
  if (data_one_norm!=actual_norm) {
    proctext << "[" << procno << "] Result " << data_one_norm
	     << " should be " << actual_norm << "\n";
  } else if (procno==0) {
    proctext << "User-defined reduction successful: " << data_one_norm << "\n";
  }
  cerr << proctext.str();

  MPI_Finalize();
  return 0;
}
