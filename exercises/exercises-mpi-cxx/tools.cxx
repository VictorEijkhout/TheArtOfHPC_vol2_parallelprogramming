/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** tools.c : support routines for the MPI exercises
 ****
 ****************************************************************/

#include <vector>
#include <string>
#include <cmath>
using namespace std;

#include "tools.h"
#include "mpi.h"

double array_error( vector<double> ref_array,vector<double> value_array) {
  int array_size = ref_array.size();
  double error = 0.,max_value=-1,min_value=-1;
  for (int i=0; i<array_size; i++) {
    double ref = ref_array[i], val = fabs(value_array[i]);
    if (min_value<0 || val<min_value) min_value = val;
    if (max_value<0 || val>max_value) max_value = val;
    double
      rel = (ref - val)/ref,
      e = fabs(rel);
    if (e>error) error = e;
  }
  return error;
};

int test_all_the_same_int( int value,MPI_Comm comm ) {
  int final_min,final_max;
  MPI_Allreduce(&value,&final_min,1,MPI_INT,MPI_MIN,comm);
  MPI_Allreduce(&value,&final_max,1,MPI_INT,MPI_MAX,comm);
  return final_min==final_max;
}

void print_final_result( int cond,MPI_Comm comm ) {
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  int error=nprocs, errors=-1;
  if (cond) 
    error = procno;
  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      printf("Finished; all results correct\n");
    else
      printf("First error occurred on proc %d\n",errors);
  }
};
