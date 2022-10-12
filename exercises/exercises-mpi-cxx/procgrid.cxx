/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise for communicator splitting
 ****
 ****************************************************************/

#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;
#include <mpi.h>

int main() {
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  stringstream proctext;

  // data specifically for this program
  int
    nrows,ncols,
    row_no,col_no,
    row_rank,col_rank;
  MPI_Comm row_comm,col_comm;

  MPI_Init(0,0);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  //
  // Try to arrange the processors in a grid
  //
  for (nrows=sqrt(nprocs+1); nrows>=1; nrows--)
    if (nprocs%nrows==0)
      break;
  if (nrows==1) {
    if (procno==0) {
      proctext << "Number of processes is prime" << endl;
      cerr << proctext.str(); proctext.clear();
    }
    MPI_Abort(comm,0);
  }
  ncols = nprocs/nrows;

  //
  // Find the coordinates of this process
  //
  row_no = procno/ncols;
  col_no = procno - ncols*row_no;

  //
  // Make a `col_comm' communicator with all processes in this column
  //
  MPI_Comm_split(comm,
/**** your code here ****/
		 );
  //
  // Make a `row_comm' communicator with all processes in this row
  //
  MPI_Comm_split(comm,
/**** your code here ****/
		 );

  //
  // Now find `row_rank': the number I am in my row
  // by using MPI_Comm_rank
  //
  MPI_Comm_rank(
/**** your code here ****/
		);
  //
  // also find `col_rank': the number I am in my column
  // by using MPI_Comm_rank
  //
  MPI_Comm_rank(
/**** your code here ****/
		);

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
  
  MPI_Finalize();
  return 0;
}
