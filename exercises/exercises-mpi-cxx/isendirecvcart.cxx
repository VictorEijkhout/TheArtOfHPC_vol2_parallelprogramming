/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2024
 ****
 **** MPI Exercise for Isend/Irecv using Cartesian topology
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "mpi.h"

#include "tools.h"

int main(int argc,char **argv) {

  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;

  int nprocs, procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  /*
   * Set up a non-periodic Cartesian grid
   */
  int dim = 2;
  int dimensions[2]; dimensions[0]=0; dimensions[1]=0;
  MPI_Dims_create(nprocs,dim,dimensions);
  if (procno==0)
    print_grid( nprocs,dim,dimensions );
  int periods[2]; periods[0]=0; periods[1]=0;
  MPI_Comm cart_comm;
  MPI_Cart_create( comm,dim,dimensions,periods,0,&cart_comm );

  double mydata=1,
    neighbor_data[4] = {0,0,0,0};
  int successor,predecessor,border_count=0;
  MPI_Request requests[8];

  // Exercise:
  // -- set `successor' and `predecessor' for all four directions
  //    note that MPI_Cart_shift gives you two neighbors
  /*
   * First horizontal
   */
/**** your code here ****/
/**** your code here ****/
      );
  // is this process on the border?
  border_count += 
    ( predecessor==MPI_PROC_NULL ? 1 : 0 ) +
    ( successor==MPI_PROC_NULL ? 1 : 0 );

  // data to the right
  MPI_Isend(&mydata,1,MPI_DOUBLE, successor,0, comm, &(requests[0]) );
  MPI_Irecv(&(neighbor_data[0]),1,MPI_DOUBLE, predecessor,0, comm, &(requests[1]) );
  // data to the left
/**** your code here ****/

  /*
   * Now vertical
   */
  MPI_Cart_shift
    ( cart_comm,
/**** your code here ****/
      );
  border_count += 
    ( predecessor==MPI_PROC_NULL ? 1 : 0 )
    + ( successor==MPI_PROC_NULL ? 1 : 0 );
/**** your code here ****/

  //
  // Now make sure all Isend/Irecv operations are completed
  //
  MPI_Waitall(8,requests,MPI_STATUSES_IGNORE);
  
  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  // check correctness
  mydata = mydata+neighbor_data[0]+neighbor_data[1]+neighbor_data[2]+neighbor_data[3];

  double res;
  if (border_count==2) {
    res = 3;
  } else if (border_count==1) {
    res = 4;
  } else {
    res = 5;
  }

  int error_test = !ISAPPROX(mydata,res);
  if (error_test)
    printf("Data on proc %d should be %e, found %e\n",procno,res,mydata);
  print_final_result(error_test,comm);

  MPI_Finalize();
  return 0;
}
