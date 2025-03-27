/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-6
 ****
 **** MPI Exercise
 ****
 ****************************************************************/

#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc,char **argv) {

  MPI_Init(0,0);
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs;
  MPI_Comm_size( comm, &nprocs );
  std::cout << "size=" << nprocs << '\n';
  MPI_Finalize();
  
  return 0;
}
