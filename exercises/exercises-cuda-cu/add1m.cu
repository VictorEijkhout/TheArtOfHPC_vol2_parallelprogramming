// -*- c -*-

#include <iostream>
#include <math.h>
#include <chrono>
using namespace std::chrono;
using myclock = steady_clock;
#include "cxxopts.hpp"

// function to add the elements of two arrays
__global__
void add1(size_t n, float *x, float *y, float *z)
{
  size_t t_id = blockIdx.x * blockDim.x + threadIdx.x;
  if (t_id>=n) return;
  z[t_id] = x[t_id] + y[t_id];
}

#define CU_ASSERT( code ) { \
  cudaError_t err = code ; \
  if (err!=cudaSuccess) { \
    printf("error <<%s>> on line %d\n",cudaGetErrorString(err),__LINE__); \
    return 1; } }

int main(int argc,char **argv) {

  cxxopts::Options options
    ( "cxxopts","Add 3 vectors with managed memory" );
  options.add_options()
    ( "h,help","usage information" )
    ( "n,nsize","size of vector", cxxopts::value<size_t>()->default_value("100") );
  auto result = options.parse(argc, argv);
  if (result.count("help")>0) {
    std::cout << options.help() << '\n';
    return 0;
  }
  /* } catch ( cxxopts::exceptions::option_has_no_value ) { */
  /*   std::cout << "Option has no value" << '\n'; */
  /* } */


  auto N = result["nsize"].as<size_t>();
  printf("Running with N=%ld\n",N);

  float
    *x, *y, *z;
  size_t nbytes = N*sizeof(float);
  CU_ASSERT( cudaMallocManaged( &x,nbytes ) );
  CU_ASSERT( cudaMallocManaged( &y,nbytes ) );
  CU_ASSERT( cudaMallocManaged( &z,nbytes ) );

  // initialize x and y arrays on the host
  for (size_t i = 0; i < N; i++) {
    x[i] = 1.0f;
    y[i] = 2.0f;
    z[i] = 0.0f;
  }

  myclock::time_point before =
    myclock::now();

  // Run kernel on 1M elements on the CPU
  int blocksize = 1024;
  int gridsize = N/blocksize+1;
  add1<<<gridsize,blocksize>>>(N,x,y,z);
  // Wait for GPU to finish before accessing data on host
  CU_ASSERT( cudaDeviceSynchronize() );

  auto after = myclock::now();
  std::cout << "Action took: "
       << duration_cast<milliseconds>
            (after-before).count()
       << "ms\n";

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (int i = 0; i < N; i++) {
    //printf("%4.1f",z[i]);
    if (z[i]!=3.0f) {
      printf("error at %d\n",i);
      break;
    }
    maxError = fmax(maxError, fabs(z[i]-3.0f));
  }
  std::cout << "Max error: " << maxError << std::endl;

  // Free memory
  // delete [] x;
  // delete [] y;
  cudaFree(x);
  cudaFree(y);

  return 0;
}
