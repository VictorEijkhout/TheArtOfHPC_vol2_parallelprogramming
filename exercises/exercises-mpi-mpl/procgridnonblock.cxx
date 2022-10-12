/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2019-2020
 ****
 **** MPI Exercise for multiple nonblocking collectives using MPL
 ****
 ****************************************************************/

#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

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
      proctext << "Number of processes is prime\n";
      cerr << proctext.str(); proctext.clear();
    }
    comm_world.abort(0);
  }
  ncols = nprocs/nrows;

  //
  // Exercise:
  // - compute the coordinates of this process
  //
  int row_no,col_no;
  row_no = procno/ncols;
  col_no = procno - ncols*row_no;

  //
  // Make a `col_comm' communicator with all processes in this column
  //
  mpl::communicator col_comm
    (mpl::communicator::split(),comm_world,col_no);

  //
  // Make a `row_comm' communicator with all processes in this row
  //
  mpl::communicator row_comm
    (mpl::communicator::split(),comm_world,row_no);

  //
  // Now find `row_rank': the number I am in my row
  //
  int row_rank = row_comm.rank();

  //
  // also find `col_rank': the number I am in my column
  //
  int col_rank = col_comm.rank();

  //
  // Let only the left column and top row query the
  // length of a row / column respectively
  //
  int col_length=0, row_length=0;
  if (row_rank==0)
    row_length = row_comm.size();
  if (col_rank==0)
    col_length = col_comm.size();

  //
  // Exercise:
  // the left column and top row tell everyone else what these lengths are.
  // Turn the broadcasts into non-blocking broadcasts.
  //
#if 0
  MPI_Bcast(&row_length,1,MPI_INT,0,row_comm);
  MPI_Bcast(&col_length,1,MPI_INT,0,col_comm);
#endif
  mpl::irequest_pool reqs;
/**** your code here ****/
  //}

  //
  // and now everyone computes how many processes there are
  //
  int grid_size = row_length * col_length;
    
  // correctness check
  int error=nprocs,errors;
  if (grid_size!=nprocs) {
    stringstream proctext;
    proctext
      << "[" << procno << "=" << row_no << "," << col_no << "] "
      << "computed wrong size: " << row_length << "x" << col_length << "=" << grid_size << "\n";
    cerr << proctext.str(); proctext.clear();
    error = procno;
  }

  comm_world.allreduce(mpl::min<int>(),error,errors);

  /*
   * Correctness test UNFINISHED 
   */
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << endl;
    else
      proctext << "First error occurred on proc " << errors << endl;
    cerr << proctext.str(); proctext.clear();
  }
  
  return 0;
}
