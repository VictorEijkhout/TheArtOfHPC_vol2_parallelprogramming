/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpi.h>

int main(int argc,char **argv) {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  stringstream proctext;
  
  // data specific for this program
  MPI_Win the_window;
  int my_number=1, my_sum = 0, window_data, other;

  MPI_Init(0,0);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  if (nprocs<3) {
    proctext << "Need at least 3 procs" << endl;
    cerr << proctext.str(); proctext.clear();
    MPI_Abort(comm,0);
  }

  /*
   * Exercise 1:
   * -- Take the variable `window_data' and make it into a window
   *    of size 1 integer
   * Exercise 2:
   * -- replace MPI_Win_create by MPI_Win_alloc
   */
  MPI_Win_create(
/**** your code here ****/
		 MPI_INFO_NULL,comm,&the_window);

  //
  // Put ten numbers from proc 0 into 1 or 2,
  // depending on random chance
  //
#define TOTALPUT 20
  for (int c=0; c<TOTALPUT; c++) {
    //
    // compute `other': where to put the data
    //
    float randomfraction = (rand() / (double)RAND_MAX);
    if (randomfraction>.5)
      other = 2;
    else other = 1;

    //
    // receiving procs set their window to zero
    //
    if (procno==1 || procno==2)
      window_data = 0;

    /*
     * Now we have an epoch where proc 0 puts data either in 1 or 2
     * Exercise: fill in the missing parameters
     */
    MPI_Win_fence(0,the_window);
    if (procno==0) {
      MPI_Put( /* data on origin: */   &my_number, 1,MPI_INT,
	       /* where are you putting it: */
/**** your code here ****/
	       the_window);
    }
    MPI_Win_fence(0,the_window);
    
    //
    // Receiving procs add incoming data (or zero) to a running sum
    //
    my_sum += window_data;
  }

  //
  // Now check that data receive on 1 & 2 together should be 20
  //
  if (procno>0 && procno<3) {
    proctext << "Sum on " << procno << ": " << my_sum << endl;
    cerr << proctext.str(); proctext.clear();
  }
  if (procno==0) {
    proctext << "Sums on 1 & 2 together should be: " << TOTALPUT << endl;
    cerr << proctext.str(); proctext.clear();
  }
    
  MPI_Win_free(&the_window);

  MPI_Finalize();
  return 0;
}
