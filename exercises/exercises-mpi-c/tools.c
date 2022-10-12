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
#include <math.h>
#include "mpi.h"

#include "tools.h"

/*
 * Return max elementwise relative error
 */
double array_error(double ref_array[],double value_array[],int array_size) {
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

/*
 * Return max elementwise error
 * for integer arrays
 */
int int_array_error(int ref_array[],int value_array[],int array_size) {
  int error = 0.,max_error=0;
  for (int i=0; i<array_size; i++) {
    int ref = ref_array[i], val = value_array[i],
      e = abs(val-ref);
    if (e>max_error) max_error = e;
  }
  return max_error;
};

/*
 * all processes have 1 integer, are they all the same?
 */
int test_all_the_same_int( int value,MPI_Comm comm ) {
  int final_min,final_max;
  MPI_Allreduce(&value,&final_min,1,MPI_INT,MPI_MIN,comm);
  MPI_Allreduce(&value,&final_max,1,MPI_INT,MPI_MAX,comm);
  return final_min==final_max;
}

/*
 * Every process has an error condition;
 * return lowest process no where condition true
 */
void print_final_result( int cond,MPI_Comm comm ) {
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  int error=nprocs, error_proc=-1;
  if (cond) 
    error = procno;
  MPI_Allreduce(&error,&error_proc,1,MPI_INT,MPI_MIN,comm);
  error_process_print(error_proc,comm);
};

void error_process_print(int error_proc, MPI_Comm comm) {
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  if (procno==0) {
    if (error_proc==nprocs) 
      printf("Finished; all results correct\n");
    else
      printf("First error occurred on proc %d\n",error_proc);
  }
}
