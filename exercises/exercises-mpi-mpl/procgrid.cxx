/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2020
 ****
 **** MPI Exercise for communicator splitting using MPL
 ****
 ****************************************************************/

#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  stringstream proctext;

  //
  // Try to arrange the processors in a grid
  //
  int nrows,ncols;
  for (nrows=sqrt(nprocs+1); nrows>=1; nrows--)
    if (nprocs%nrows==0)
      break;
  if (nrows==1) {
    if (procno==0) {
      proctext << "Number of processes is prime" << endl;
      cerr << proctext.str(); proctext.clear();
    }
    comm_world.abort(0);
  }
  ncols = nprocs/nrows;

  //
  // Find the coordinates of this process
  //
  int row_no,col_no;
  row_no = procno/ncols;
  col_no = procno - ncols*row_no;

  //
  // Make a `col_comm' communicator with all processes in this column
  //
  mpl::communicator col_comm
    (mpl::communicator::split(),comm_world,
/**** your code here ****/
     );

  //
  // Make a `row_comm' communicator with all processes in this row
  //
  mpl::communicator row_comm
    (mpl::communicator::split(),comm_world,
/**** your code here ****/
     );

  //
  // Now find `row_rank': the number I am in my row
  // by using MPI_Comm_rank
  //
  int row_rank =
/**** your code here ****/
    ;

  //
  // also find `col_rank': the number I am in my column
  // by using MPI_Comm_rank
  //
  int col_rank =
/**** your code here ****/
    ;

  //
  // Now check that the rank in the row equals the column number
  //
  if (row_rank!=col_no) {
    proctext << "[" << procno << "=" << row_no << "," << col_no << "] wrong row rank "
	     << row_rank << endl;
    cerr << proctext.str(); proctext.clear();
  }

  //
  // Now check that the rank in the column equals the row number
  //
  if (col_rank!=row_no) {
    proctext << "[" << procno << "=" << row_no << "," << col_no << "] wrong col rank "
	     << col_rank << endl;
    cerr << proctext.str(); proctext.clear();
  }

  if (procno==0) {
    proctext << "Finished" << endl;
    cerr << proctext.str(); proctext.clear();
  }
  
  return 0;
}
