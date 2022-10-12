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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "mpi.h"

#ifndef N
#define N 10
#endif

/*
 * Compare local data `mydata' against incoming `otherdata'
 * Set local data to negative if it occurs in the incoming
 */
void setdiff( int *mydata,int *otherdata,int n ) {
  for ( int i_my=0; i_my<n; i_my++) {
    int my = mydata[i_my];
    for ( int i_ot=0; i_ot<n; i_ot++ ) {
      int ot = otherdata[i_ot];
      if (my==ot) { mydata[i_my] = -1; break; }
    }
  }
}

/*
 * Print the filter array by first gathering it
 * Note: this is not a scalable thing to do.
 */
void print_distarray( int *data,int n,MPI_Comm comm ,char *txt,int sort) {
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  int *global_data = procno==0 ? (int*) malloc( nprocs*n*sizeof(int) ) : NULL;
  MPI_Gather( data,n,MPI_INT,global_data,n,MPI_INT,0,comm);
  if (procno==0) {
    if (sort) {
      for (int top=nprocs*n-1; top>=0; top--) {
	for (int s=0; s<top; s++) {
	  if (global_data[s+1]<global_data[s]) {
	    int t = global_data[s+1];
	    global_data[s+1] = global_data[s];
	    global_data[s] = t;
	  }
	}
      }
    }
    printf("%s",txt);
    for (int i=0; i<nprocs*n; i++) printf(" %3d",global_data[i]);
    printf("\n");
    free(global_data);
  }
}

int main(int argc,char **argv) {

  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;

  int nprocs, procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  // Initialize the random number generator
  srand(procno);

  // Set `sendto' and `recvfrom'
  int sendto   = ( procno+1 ) % nprocs;
  int recvfrom = ( procno-1+nprocs ) % nprocs;

  /*
   * Generate random local and other data
   */
  int mydata[N], filter[N],result[N];
  for (int i=0; i<N; i++) {
    mydata[i] = rand() % (2*N*nprocs);
    result[i] = mydata[i];
    filter[i] = rand() % (2*N*nprocs);
  }

  print_distarray(mydata,N,comm,"Starting : ",0);
  print_distarray(filter,N,comm,"Filtering: ",1);

  /*
   * Exercise:
   * -- compute the global set difference 
   *    by passing the filter around the ring in a bucket brigade
   */
  for (int p=0; p<nprocs; p++) {
/**** your code here ****/
    setdiff( result,filter,N );
/**** your code here ****/
  }
 
  print_distarray(result,N,comm,"Ending   : ",0);

  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  double max_of_errors = 0;
  int
    error = max_of_errors > 1.e-12 ? procno : nprocs,
    errors=-1;
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
