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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vector>
using std::vector;

#include <cassert>
#include <cmath>

#include <mpl/mpl.hpp>
#include "tools.h"

bool isapprox(int x,int y) {
  return x==y;
};
bool isapprox(double x,double y) {
 return 
   ( ( x==0. && fabs(y)<1.e-14 ) || ( y==0. && fabs(x)<1.e-14 ) ||	\
     fabs(x-y)/fabs(x)<1.e-14 );
};
bool isapprox(float x,float y) {
 return 
   ( ( x==0. && fabs(y)<1.e-14 ) || ( y==0. && fabs(x)<1.e-14 ) ||	\
     fabs(x-y)/fabs(x)<1.e-14 );
};

double array_error(const vector<double>& ref_array,const vector<double>& value_array) {
  int array_size = ref_array.size();
  assert(array_size==value_array.size());
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

int test_all_the_same_int( int value,const mpl::communicator& comm ) {
  int final_min,final_max;
  comm.allreduce(mpl::min<int>(),value,final_min);
  //  MPI_Allreduce(&value,&final_min,1,MPI_INT,MPI_MIN,comm);
  comm.allreduce(mpl::max<int>(),value,final_max);
  // MPI_Allreduce(&value,&final_max,1,MPI_INT,MPI_MAX,comm);
  return final_min==final_max;
}

void print_final_result( bool cond,const mpl::communicator& comm ) {
  int nprocs,procno;
  nprocs = comm.size();
  procno = comm.rank();
  int error=nprocs, error_proc=-1;
  if (cond) 
    error = procno;
  comm.allreduce(mpl::min<int>(),error,error_proc);
  error_process_print(error_proc,comm);
};

void error_process_print(int error_proc, const mpl::communicator& comm) {
  int nprocs,procno;
  nprocs = comm.size();
  procno = comm.rank();
  if (procno==0) {
    if (error_proc==nprocs) 
      printf("Finished; all results correct\n");
    else
      printf("First error occurred on proc %d\n",error_proc);
  }
}
