/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2023
   %%%%
   %%%% irecv_source.cxx : combine Irecv and Waitany
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
    //codesnippet mpirequestpush
    mpl::irequest_pool recv_requests;
    //codesnippet end
    vector<int> recv_buffer(nprocs-1);
    //codesnippet mpirequestpush
    for (int p=0; p<nprocs-1; p++) {
      recv_requests.push( comm_world.irecv( recv_buffer[p], p ) );
    }
    //codesnippet end
    printf("Outstanding request #=%lu\n",recv_requests.size());
    for (int p=0; p<nprocs-1; p++) {
      //codesnippet waitforanympl
      auto [success,index] = recv_requests.waitany();
      if ( success==mpl::test_result::completed ) {
    	auto recv_status = recv_requests.get_status(index);
	//codesnippet end
    	int source = recv_status.source();
    	if (index!=source)
    	  printf("Mismatch index %lu vs source %d\n",index,source);
    	printf("Message from %lu: %d\n",index,recv_buffer[index]);
      } else
    	break;
    }
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
