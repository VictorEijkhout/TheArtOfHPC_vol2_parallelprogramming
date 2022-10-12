/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2019
 ****
 **** MPI Exercise for multiple nonblocking collectives
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
    row_rank,col_rank;
  MPI_Comm row_comm,col_comm;

  //
  // Try to arrange the processes in a grid
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
  // Compute the coordinates of this process
  //
  int row_no,col_no;
  row_no = procno/ncols;
  col_no = procno - ncols*row_no;

  //
  // Make a `col_comm' communicator with all processes in this column
  //
  MPI_Comm_split(comm,col_no,0,&col_comm);
  //
  // Make a `row_comm' communicator with all processes in this row
  //
  MPI_Comm_split(comm,row_no,0,&row_comm);

  //
  // Now find `row_rank': the number I am in my row
  //
  MPI_Comm_rank(row_comm,&row_rank);
  //
  // also find `col_rank': the number I am in my column
  //
  MPI_Comm_rank(col_comm,&col_rank);

  //
  // Let only the left column and top row query the
  // length of a row / column respectively
  //
  int col_length=0, row_length=0;
  if (row_rank==0)
    MPI_Comm_size(row_comm,&row_length);
  if (col_rank==0)
    MPI_Comm_size(col_comm,&col_length);

  //
  // Exercise:
  // the left column and top row tell everyone else
  // what these lengths are.
  //
#if 0
  // replace these blocking calls by non-blocking
  MPI_Bcast(&row_length,1,MPI_INT,0,row_comm);
  MPI_Bcast(&col_length,1,MPI_INT,0,col_comm);
#endif
/**** your code here ****/

  //
  // and now everyone computes how many processes there are
  //
  int grid_size = row_length * col_length;
    
  // correctness check
  int error=nprocs,errors;
  if (grid_size!=nprocs) {
    printf("[%d=%d,%d] computed wrong size: %dx%d=%d\n",
	   procno,row_no,col_no,row_length,col_length,grid_size);
    error = procno;
  }

  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      printf("Finished; all results correct\n");
    else
      printf("First error occurred on proc %d\n",errors);
  }
  
  MPI_Finalize();
  return 0;
}
