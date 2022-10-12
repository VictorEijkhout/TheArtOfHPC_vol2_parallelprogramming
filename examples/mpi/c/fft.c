/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% fft.c : fft exercise
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <mpi.h>

static double pi = 3.141592653589793238462643383279502884;

void set_sine( MPI_Comm comm,double *vector,int localsize,int freq ) {
  int globalsize;
  MPI_Allreduce( &localsize,&globalsize,1,MPI_INT,MPI_SUM, comm );
  globalsize += 1;
  int myfirst=0;
  MPI_Exscan( &localsize,&myfirst,1,MPI_INT,MPI_SUM, comm );

  for (int i=0; i<localsize; i++)
    vector[i] = sin( pi*freq* (i+1+myfirst) / globalsize );
};

double inprod( MPI_Comm comm,double *v1,double *v2,int s ) {
  double vlocal=0., vglobal;
  for ( int i=0; i<s; i++ )
    vlocal += v1[i]*v2[i];
  MPI_Allreduce( &vlocal,&vglobal,1,MPI_DOUBLE,MPI_SUM, comm );
  int N;
  MPI_Allreduce( &s,&N,1,MPI_INT,MPI_SUM, comm );
  N += 1;
  vglobal /= N/2;
  return vglobal;
};

int main() {

  MPI_Init(0,0);
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  int nlocal = 10+procno;
  double
    *sine  = (double*) malloc( nlocal*sizeof(double) ),
    *sine2 = (double*) malloc( nlocal*sizeof(double) ),
    *sine5 = (double*) malloc( nlocal*sizeof(double) );
  
  set_sine( comm,sine, nlocal,1);
  set_sine( comm,sine2,nlocal,2);
  set_sine( comm,sine5,nlocal,5);

  double ortho;
  ortho = inprod( comm,sine,sine,nlocal );
  if (procno==0)
    printf("Inner product 1x1=%e\n",ortho);

  ortho = inprod( comm,sine,sine2,nlocal );
  if (procno==0)
    printf("Inner product 1x2=%e\n",ortho);

  ortho = inprod( comm,sine2,sine2,nlocal );
  if (procno==0)
    printf("Inner product 2x2=%e\n",ortho);

  ortho = inprod( comm,sine5,sine2,nlocal );
  if (procno==0)
    printf("Inner product 2x5=%e\n",ortho);

  free(sine); free(sine2); free(sine5);

  MPI_Finalize();

  return 0;
}
