/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% jordancol.c : Gauss-Jordan with matrix distributed by columns using MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <cmath>
using namespace std;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs, procno;
  
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();
  
  // set the default random number generator
  std::default_random_engine generator;
  // distribution: real between 0 and 1
  std::uniform_real_distribution<float> distribution(0.,1.);

  /*
   * We set the matrix size to the number of processes.
   * Each process allocates
   * - one column of the matrix
   * - a scaling vector
   * - redundantly, the solution and right-hand side
   */
  int N = nprocs;
  vector<double> 
    matrix(N),
    solution(N), rhs(N), scalings(N);

  /*
   * Fill the matrix with random data
   * and increase the diagonal for numerical stability
   */
  for (int row=0; row<N; row++) {
    matrix.at(row) = distribution(generator);
    if (row==procno)
      matrix.at(row) += .5;
  }
  
  /*
   * Set the right-hand side to row sums,
   * so that the solution is identically one.
   * We use one long reduction.
   */
  comm_world.allreduce
    (mpl::plus<double>(),matrix.data(),rhs.data(),mpl::contiguous_layout<double>(N));

  /*
   * Now iterate over the columns, 
   * using the diagonal element as pivot.
   */
  for (int pivot_column=0; pivot_column<N; pivot_column++) {
    if (pivot_column==procno) {
      double pivot = matrix[pivot_column];
      // scaling factors per row
      for (int row=0; row<N; row++)
	scalings.at(row) = matrix.at(row)/pivot;
    }
    /*
     * Exercise:
     * make sure that everyone knows the scaling factors
     */
/**** your code here ****/
    /*
     * Now update the matrix.
     * Answer for yourself: why is there no loop over the columns?
     */
    for (int row=0; row<N; row++) {
      if (row==pivot_column) continue;
      matrix[row] =
	matrix[row] - scalings[row]*matrix[pivot_column];
    }
    // update the right hand side
    for (int row=0; row<N; row++) {
      if (row==pivot_column) continue;
      rhs[row] = rhs[row] - scalings[row]*rhs[pivot_column];
    }
  }

  // check that we have swept
  for (int row=0; row<N; row++) {
    stringstream proctext;
    if (row==procno) continue;
    if (abs(matrix[row])>1.e-13) {
      proctext << "Wrong value at [" << row << "," << procno << "]:" << matrix[row] << endl;
      cerr << proctext.str(); proctext.clear();
    }
  }

  // solve the system
  double local_solution = rhs.at(procno)/matrix.at(procno);
  comm_world.allgather( local_solution,solution.data() );
  //  MPI_Allgather(&local_solution,1,MPI_DOUBLE,solution,1,MPI_DOUBLE,comm);

  // check correct solution
  if (procno==0) {
    stringstream proctext;
    bool success{true};
    for (int row=0; row<N; row++)
      if ( abs(solution[row]-1.)>1.e-13 ) {
	proctext << "Wrong solution at [" << row << "]:" << solution[row] << endl;
	cerr << proctext.str(); proctext.clear();
	success = false;
      }
    if (success)
      proctext << "Success" << endl;
    else
      proctext << "Failure" << endl;
    cerr << proctext.str();
  }

  return 0;
}
