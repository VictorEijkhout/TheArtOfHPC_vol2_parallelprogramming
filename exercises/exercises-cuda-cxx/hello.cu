// -*- c -*-
/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2016-2025 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** hello.cxx : single thread CUDA hello world
 ****
 ****************************************************************/

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>

//codesnippet cuhellodef
__global__ void hello_cuda() {
  printf("hello world!\n");
}
//codesnippet end

int main() {
  //codesnippet cuhellouse
  hello_cuda<<<1,1>>>();
  cudaDeviceSynchronize();
  //codesnippet end

  return 0;
}
