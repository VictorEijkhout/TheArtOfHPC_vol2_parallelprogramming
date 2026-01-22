/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise for prime number factorization. So to speak.
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

  int bignum = 2000000111, maxfactor = 45200;
  // Exercise:
  // -- Parallelize the do loop so that each processor
  //    tries different candidate numbers.
  // -- If a processors finds a factor, print it to the screen.

  // 1. Set loop bounds
  int blocksize = maxfactor/nprocs;
  int myfirst = procno*blocksize,
    mylast = (procno+1)*blocksize-1 ;
  if (procno==0) myfirst = 2;
  if (procno==nprocs-1) mylast = maxfactor;
  // 2. Fill in loop header
  for ( int myfactor=
	  // this is sequential: 2; myfactor<bignum;
	  myfirst; myfactor<mylast;
	myfactor++
       ) {
    if (bignum%myfactor==0) {
      printf("Processor %d found factor %d\n",procno,myfactor);
    }
  }
  
  MPI_Finalize();
  return 0;
}
