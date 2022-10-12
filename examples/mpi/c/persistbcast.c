/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% persistbcast.c : test of persistent collectivese
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  // What's the local size of the vector
  int nlocal = 1000;
  int niter = 100;
  if (argc>1) nlocal = atoi(argv[1]);

  double *vector = (double*) malloc( nlocal*sizeof(double) );
  for (int i=0; i<nlocal; i++) vector[i] = 1.;
  double *matrix = (double*) malloc( nlocal*sizeof(double) );
  for (int i=0; i<nlocal; i++) matrix[i] = (double)rand()/RAND_MAX;

  double max_ev=0;
#define max(a,b) (a)>(b) ? (a) : (b)
  for (int i=0; i<nlocal; i++) max_ev = max( matrix[i],max_ev );
  MPI_Allreduce( MPI_IN_PLACE,&max_ev,1,MPI_DOUBLE,MPI_MAX,comm );
  if (procno==0) printf("Max ev: %e\n",max_ev);

  double how_close=0;
  double norm = 0.;
#define VARIANT 2
  for (int it=0; it<niter; it++) {
    // matrix vector multiplication
    for (int i=0; i<nlocal; i++) vector[i] *= matrix[i];

#if VARIANT == 1
    // compute norm
    norm  = 0.;
    for (int i=0; i<nlocal; i++) norm += vector[i]*vector[i];
    MPI_Allreduce( MPI_IN_PLACE,&norm,1,MPI_DOUBLE,MPI_SUM,comm );
    norm = sqrt(norm);
    if (procno==0) printf("1. ev=%e / %e\n",norm,max_ev);
    how_close = norm/max_ev;
    // normalize
    for (int i=0; i<nlocal; i++) vector[i] /= norm;
#elif VARIANT == 2
    // normalize
    if (it>0)
      for (int i=0; i<nlocal; i++) vector[i] /= norm;
    // compute norm
    norm  = 0.;
    for (int i=0; i<nlocal; i++) norm += vector[i]*vector[i];
    MPI_Allreduce( MPI_IN_PLACE,&norm,1,MPI_DOUBLE,MPI_SUM,comm );
    norm = sqrt(norm);
    if (procno==0) printf("2. ev=%e / %e\n",norm,max_ev);
    how_close = norm/max_ev;
#else
#error Unknown variant
#endif
  }
  if (procno==0) printf("After %d iterations, reached %e fraction\n",niter,how_close);

  MPI_Request requests[2];

  MPI_Finalize();
  return 0;
}
