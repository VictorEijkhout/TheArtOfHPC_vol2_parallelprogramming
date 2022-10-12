/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise for communicator splitting
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc,char **argv) {
  
  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;

  int nprocs, procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  // data specifically for this program
  int
    nrows,ncols,
    rank_in_row,rank_in_col;
  MPI_Comm row_comm,col_comm;

  //
  // Try to arrange the processors in a grid
  //
  for (nrows=sqrt(nprocs+1); nrows>=1; nrows--)
    if (nprocs%nrows==0)
      break;
  if (nrows==1) {
    if (procno==0)
      printf("Number of processes is prime\n");
    MPI_Abort(comm,0);
  }
  ncols = nprocs/nrows;

  //
  // Exercise:
  // - compute the coordinates of this process
  //
  int row_no,col_no;
/**** your code here ****/

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
  // Now find `rank_in_row': the number I am in my row
  // by using MPI_Comm_rank
  //
  MPI_Comm_rank(
/**** your code here ****/
		);
  //
  // also find `rank_in_col': the number I am in my column
  // by using MPI_Comm_rank
  //
  MPI_Comm_rank(
/**** your code here ****/
		);

  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  int error=nprocs,errors;

  //
  // Now check that the rank in the row equals the column number
  //
  if (rank_in_row!=col_no) {
    printf("[%d=%d,%d] wrong row rank %d\n",
	   procno,row_no,col_no,rank_in_row);
    error = procno;
  }
  //
  // Now check that the rank in the column equals the row number
  //
  if (rank_in_col!=row_no) {
    printf("[%d=%d,%d] wrong col rank %d\n",
	   procno,row_no,col_no,rank_in_col);
    error = procno;
  }

  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      printf("Part 1 finished; all results correct\n");
    else
      printf("Part 1: First error occurred on proc %d\n",errors);
  }
  
  MPI_Finalize();
  return 0;
}
