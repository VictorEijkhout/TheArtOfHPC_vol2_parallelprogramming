/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** MPI Exercise to illustrate pipelining
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <mpi.h>

// headers just for this exercise
#include <algorithm>
#include <vector>

using namespace std;

#ifndef N
#define N 10
#endif

int main(int argc,char **argv) {

  stringstream proctext;

  MPI_Init(&argc,&argv);

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);


  // Set `sendto' and `recvfrom'
  int sendto =
    ( procno<nprocs-1 ? procno+1 : MPI_PROC_NULL )
    ;
  int recvfrom =
    ( procno>0 ? procno-1 : MPI_PROC_NULL )
    ;

  //
  // Exercise:
  // -- do the MPI_Send and MPI_Recv calls
  //
  vector<double> leftdata(N,0.), myvalue(N,0.);
/**** your code here ****/


  /*
   * Check correctness
   */
  double p1 = procno+1.;
  double my_sum_of_squares = p1*p1*p1/3 + p1*p1/2 + p1/6;
  double max_of_errors = 0;
  for (int i=0; i<N; i++) {
    double e = fabs( (my_sum_of_squares - myvalue[i])/my_sum_of_squares );
    if (e>max_of_errors) max_of_errors = e;
  }
  int
    error = max_of_errors > 1.e-12 ? procno : nprocs,
    errors=-1;
  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << endl;
    else
      proctext << "First error occurred on proc " << errors << endl;
    cout << proctext.str(); proctext.clear();
  }

  MPI_Finalize();
  return 0;
}
