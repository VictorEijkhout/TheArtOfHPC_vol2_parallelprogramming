// -*- c++ -*-
/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2016-2025 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** device.cxx : dealing with cuda devices
 ****
 ****************************************************************/

#include <iostream>
using std::cout;

int main() {

  const int ndev = 
    [] () ->int {
      //codesnippet cudevcount
      int ndev;
      auto status = cudaGetDeviceCount(&ndev);
      //codesnippet end
      if (status==cudaSuccess)
	return ndev;
      else if (status==cudaErrorNoDevice) {
	cout << "No devices found\n"; throw status;
      } else if (status==cudaErrorInsufficientDriver) {
	cout << "Insufficient driver\n"; throw status;
      } else return 0; }();

  cout << "Number of devices detected: " << ndev << '\n';
  for ( int idev=0; idev<ndev; ++idev ) {
    //codesnippet cudevprop
    cudaDeviceProp properties;
    cudaGetDeviceProperties(&properties,idev);
    //codesnippet end
    //codesnippet cudevprops
    cout << "Device " << idev << "=" << properties.name << '\n';
    cout << "  async: " << properties.asyncEngineCount << '\n';
    cout << "  unified: " << properties.unifiedAddressing << '\n';

    cout << "  capability: " << properties.major << "." << properties.minor << '\n';
    cout << "  multiprocs: " << properties.multiProcessorCount << '\n';
    cout << "  clock rate: " << properties.clockRate << '\n';

    cout << "  global memory: " << properties.totalGlobalMem << '\n';
    cout << "  shared mem/block: " << properties.sharedMemPerBlock << '\n';

    cout << "  max threads/block: " << properties.maxThreadsPerBlock << '\n';
    cout << "  max thread dims: " << properties.maxThreadsDim[0] << ","
	 << properties.maxThreadsDim[1] << "," 
	 << properties.maxThreadsDim[2] << '\n';
    //codesnippet end
  }

#if CUDA_VERSION >= 12000
  cudaInitDevice(0,0,0);
#endif

  return 0;
}
