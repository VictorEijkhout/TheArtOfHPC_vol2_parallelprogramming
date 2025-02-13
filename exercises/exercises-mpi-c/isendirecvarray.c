/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2025
 ****
 **** MPI Exercise for Isend/Irecv, sending an array
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

#define N 100
  int mylocal = N;
  double
    *indata = (double*)malloc(mylocal*sizeof(double)),
    *outdata = (double*)malloc(mylocal*sizeof(double));
  int myfirst=0;
  MPI_Exscan( &mylocal,&myfirst,1,MPI_INT,MPI_SUM,comm);
  for (int i=0; i<mylocal; i++)
    indata[i] = myfirst+i;

  double
    leftdata=0.,rightdata=0.;
  int sendto,recvfrom;
  MPI_Request requests[4];

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  //    once to get data from the left, once from the right
  // -- for first/last process use MPI_PROC_NULL

  // first specify left neighbor data
/**** your code here ****/
  MPI_Isend( // send data to the left
/**** your code here ****/
	    1,MPI_DOUBLE, sendto,0, comm,&(requests[0]));
  MPI_Irecv(&leftdata,1,MPI_DOUBLE, recvfrom,0, comm,&(requests[1]));

  // then the right neighbor data
/**** your code here ****/
  MPI_Isend( // send data to the right
/**** your code here ****/
	    1,MPI_DOUBLE, sendto,0, comm,&(requests[2]));
  MPI_Irecv(&rightdata,1,MPI_DOUBLE, recvfrom,0, comm,&(requests[3]));

  // make sure all irecv/isend operations are concluded
/**** your code here ****/

  /*
   * Do the averaging operation
   * Note that leftdata==rightdata==0 if not explicitly received.
   */
  for (int i=0; i<mylocal; i++)
    if (i==0)
      outdata[i] = leftdata + indata[i] + indata[i+1];
    else if (i==mylocal-1)
      outdata[i] = indata[i-1] + indata[i] + rightdata;
    else
      outdata[i] = indata[i-1] + indata[i] + indata[i+1];
  
  /*
   * Check correctness of the result:
   * value should be 3 times the invalue, except at the end points
   */
  double *answer = (double*)malloc(mylocal*sizeof(double));
  for (int i=0; i<mylocal; i++) {
    answer[i] = 3*(myfirst+i)
      - ( procno==0 && i==0 ) * (-1) // fictitious leftleft elements
      - ( procno==nprocs-1 && i==mylocal-1) * (myfirst+mylocal) // rightright
      ;
  }
  double error_test = array_error(answer,outdata,mylocal);
  print_final_result( error_test>nprocs*1.e-14,comm);

  free(indata); free(outdata); free(answer);

  MPI_Finalize();
  return 0;
}
