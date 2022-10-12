/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2022
 ****
 **** pingpong with big counts
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>
#include <math.h>
#include <string.h>

#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;

  int nprocs, procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

#define NEXPERIMENTS 10
  int processA,processB;
  processA = 0; processB = nprocs-1;
  if (procno==0) 
    printf("Ping-pong between ranks %d--%d, repeated %d times\n",
           processA,processB,NEXPERIMENTS);

  /*
   * Do a loop over sizes, up to 10 billion eleemnts
   */
  assert( sizeof(MPI_Count)>4 );
  if (procno==0) {
    printf("MPI Count has %lu bytes\n",sizeof(MPI_Count));
  }
  for ( int power=3; power<=10; power++) {
    MPI_Count length=pow(10,power);

    /*
     * Try to tune how many experiments
     */
    int nexp_power = 1+(10-power)/2;
    if (nexp_power>5) nexp_power = 5;
    int nexperiments = pow(10,nexp_power);
    if (procno==0)
      printf("Size: 10^%d, (repeats=%d)\n",power,nexperiments);

    double *buffer;
    if (procno==processA || procno==processB) {
      buffer = (double*)malloc( length*sizeof(double) );
      if (!buffer) {
        printf("Could not allocate buffer\n"); MPI_Abort(comm,0);
      }
      for ( size_t i=0; i<length; i++ )
        buffer[i] = i;
    }

    /*
     * Process A does ping and time measurement
     */
    if (procno==processA) {
      double t = MPI_Wtime();
      for (int n=0; n<nexperiments; n++) {
        MPI_Ssend_c
          (buffer,length,MPI_DOUBLE,
           processB,0,comm);
        MPI_Recv_c
          (buffer,length,MPI_DOUBLE,
           processB,0,comm,MPI_STATUS_IGNORE);
      }
      t = MPI_Wtime()-t; t /= nexperiments;
      {
        int usec = t*pow(10,6);
        float gbs = 2 * 1.e-9 * length * sizeof(double) / t;
        printf("Time %e for size 10^%d: %6.4f Gb/sec\n",
               t,power,gbs);
      }
    } else if (procno==processB) {
      for (int n=0; n<nexperiments; n++) {
        MPI_Recv_c
          (buffer,length,MPI_DOUBLE,
             processA,0,comm,MPI_STATUS_IGNORE);
        MPI_Ssend_c
          (buffer,length,MPI_DOUBLE,
           processA,0,comm);
      }
    }
  }

  MPI_Finalize();
  return 0;
}
