/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 **** sumsquares.c
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv) {
  
  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;

  int nprocs, procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  /*
   * You can call this program as "sumsquare 12345"
   * where the numeric argument is the number of elements
   * on each processor. There is a default if you don't
   * specify this.
   */
  int nlocal,nglobal;
  if (procno==0) {
    if (argc>1)
      nlocal = atoi(argv[1]);
    else nlocal = 1000;
  }
  MPI_Bcast(&nlocal,1,MPI_INTEGER,0,comm);
  nglobal = nprocs*nlocal;

  // Allocate local data. We use doubles rather than integers (why?)
  double *local_squares = (double*)malloc(nlocal*sizeof(double));

  /*
   * Exercise part 1:
   * -- set your local values
   */

  for (int i=0; i<nlocal; i++) {
/**** your code here ****/
  }

  /*
   * Exercise part 2:
   * -- do the local summation
   * -- do the global summation
   */
  double local_sum = 0., global_sum;
  for (int i=0; i<nlocal; i++) {
/**** your code here ****/
  }
  MPI_Reduce(&local_sum,&global_sum,1,MPI_DOUBLE,
	     /* operator: */
/**** your code here ****/
	     0,comm);
  if (procno==0) {
    double fglobal = nglobal;
    printf("Global sum: %e should be %e\n",
	   global_sum,fglobal*fglobal*fglobal/3. + fglobal*fglobal/2. + fglobal/6.);
  }

  MPI_Finalize();
  return 0;
}
