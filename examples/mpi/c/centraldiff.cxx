/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% centraldiff.tex : 1D central differences through one-sided operations
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

double test_convergence(double *local,int local_size,int global_size,
			int first, MPI_Comm comm);
int average(double local[],double halo[],int localsize,MPI_Win window,MPI_Comm comm);

int main(int argc,char ** argv) {

#include "globalinit.c"

  int localsize=10, globalsize;
  int my_first=0;
  err = MPI_Allreduce(&localsize,&globalsize,
              1,MPI_INT,MPI_SUM,comm); CHK(err);
  err = MPI_Exscan(&localsize,&my_first,
              1,MPI_INT,MPI_SUM,comm); CHK(err);
  double *local_domain = new double[localsize],
    *halo_domain = new double[localsize+2];
  for (int i=0; i<localsize; i++)
    local_domain[i] = (double)procno;

  MPI_Win my_window;
  err = MPI_Win_create(local_domain,
       	       localsize*sizeof(double),sizeof(double),MPI_INFO_NULL,
       	       comm, &my_window); CHK(err);
  for (int iter=0; iter<100; iter++) {
    double dev;
    average(local_domain,halo_domain,localsize,my_window,comm);
    dev = test_convergence(local_domain,localsize,globalsize,my_first,comm);
    if (procno==0) printf("Deviation %8.5e\n",dev);
  }

  err = MPI_Win_free(&my_window); CHK(err);
  err = MPI_Finalize(); CHK(err);
  return 0;
}

double test_convergence(double *local,int localsize,int global_size,
			int first, MPI_Comm comm) {
  double local_deviation=0.,global_deviation;
  for (int i=0; i<localsize; i++) {
    int global_coordinate = (first+i) * (1./(global_size+1));
    double dev = abs( double (local[i]-1+global_coordinate) );
    local_deviation += dev*dev;
  }
  MPI_Reduce(&local_deviation,&global_deviation,1,MPI_DOUBLE,MPI_SUM,0,comm);
  return sqrt(global_deviation);
}

int average(double local[],double halo[],int localsize,MPI_Win window,MPI_Comm comm) {
  int nprocs,procno,err; double left_val,right_val;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  printf("%d %d\n",procno,nprocs);
  for (int i=0; i<localsize; i++)
    halo[i+1] = local[i];
  err = MPI_Win_fence(MPI_MODE_NOPRECEDE,window); CHK(err);
  // get from the left
  if (procno>0) {
    err = MPI_Get(&left_val,1,MPI_DOUBLE,
            procno-1, // get from
            localsize-1,1,MPI_DOUBLE, // offset and amount
            window); CHK(err);
  } else left_val = 1.;
  // get from the right
  if (procno<nprocs-1) {
    err = MPI_Get(&right_val,1,MPI_DOUBLE,
            procno+1, // get from
            0,1,MPI_DOUBLE, // offset and amount
            window); CHK(err);
  } else right_val= 0.;
  err = MPI_Win_fence(MPI_MODE_NOSUCCEED,window); CHK(err);
  halo[0] = left_val; halo[localsize+1] = right_val;
  // printf("%d: ",procno);
  // for (int i=0; i<localsize+2; i++) printf("%e ",halo[i]);
  // printf("\n");
  for (int i=0; i<localsize; i++)
    local[i] = 2*halo[i+1]-halo[i+2]-halo[i];
  return 0;
}

