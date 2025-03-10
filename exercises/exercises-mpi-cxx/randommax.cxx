/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2025
 ****
 **** MPI Exercise
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <random>
using namespace std;
#include <mpi.h>

int main() {
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Init(0,0);

  // compute communicator rank and size
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  
  // Create a random number generator seeded by the process number
  std::random_device rd;  // Used to get a random seed
  std::mt19937 gen(rd() + procno);  // Mersenne Twister generator
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);  // Distribution in range [0,1)
  
  // Compute a normalized random number
  float myrandom = dist(gen), globalrandom;
  {
    stringstream proctext;
    proctext << "Process " << setw(3) << procno << " has random value " << myrandom << endl;
    cerr << proctext.str(); proctext.clear();
  }

  /*
   * Exercise part 1:
   * -- compute the sum of the values, everywhere
   * -- scale your number by the sum
   * -- check that the sum of scales values is 1
   */
  float sum_random, scaled_random, sum_scaled_random;
  MPI_Allreduce(
/**** your code here ****/
		);
  scaled_random = myrandom / sum_random;
  MPI_Allreduce(
/**** your code here ****/
		);
  /*
   * Correctness test
   */
  int error=nprocs, errors;
  if ( abs(sum_scaled_random-1.)>1.e-5 ) {
    stringstream proctext;
    proctext << "Suspicious sum " << sum_random << " on process " << procno << endl;
    cerr << proctext.str(); proctext.clear();
    error = procno;
  }
  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    stringstream proctext;
    if (errors==nprocs) 
      proctext << "Part 1 finished; all results correct" << endl;
    else
      proctext << "Part 1: first error occurred on rank " << errors << endl;
    cerr << proctext.str(); proctext.clear();
  }

#if 0
  // Exercise part 2:
  // -- compute the maximum random value on process zero
  MPI_Reduce(
/**** your code here ****/
	     );
  if (procno==0) {
    stringstream proctext;
    proctext << "Part 2: The maximum number is " << globalrandom << endl;
    cerr << proctext.str(); proctext.clear();
  }
#endif

  MPI_Finalize();
  return 0;
}
