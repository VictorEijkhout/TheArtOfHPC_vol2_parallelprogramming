/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2025
   %%%%
   %%%% irecvall.cxx : combine Irecv and Waitall
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <unistd.h>
#include <complex>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <cassert>

#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world=mpl::environment::comm_world();
  int
    nprocs = comm_world.size(),
    procno = comm_world.rank();
  if (comm_world.size()<2)
    return EXIT_FAILURE;

  // Initialize the random number generator
  srand((int)(procno*(double)RAND_MAX/nprocs));

  if (procno==nprocs-1) {
    mpl::irequest_pool recv_requests;
    vector<int> recv_buffer(nprocs-1);
    for (int p=0; p<nprocs-1; p++) {
      recv_requests.push( comm_world.irecv( recv_buffer[p], p ) );
    }
    printf("Outstanding request #=%lu\n",recv_requests.size());
    //codesnippet waitforallmpl
    recv_requests.waitall();
    auto status_last = recv_requests.get_status( recv_requests.size()-1 );
    //codesnippet end
  } else {
    float randomfraction = (rand() / (double)RAND_MAX);
    int randomwait = (int) ( 2* nprocs * randomfraction );
    printf("process %d waits for %d\n",procno,randomwait);
    sleep(randomwait);
    comm_world.send( procno, nprocs-1 );
  }

  return 0;
}
