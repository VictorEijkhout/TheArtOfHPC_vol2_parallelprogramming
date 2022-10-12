/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% indexed.cxx : MPI_Type_indexed in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <cassert>

#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  // MPI Comm world
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  int nprocs = comm_world.size(), procno = comm_world.rank();

  int sender = 0, receiver = 1, the_other = 1-procno,
    totalcount = 15, targetbuffersize = 2*totalcount;

  vector<int>
    source_buffer(totalcount),
    target_buffer(targetbuffersize);
  for (int i=0; i<totalcount; ++i)
    source_buffer[i] = i;

  const int count = 5;
  mpl::contiguous_layout<int>
    fiveints(count);
  mpl::indexed_layout<int>
    indexed_where{ { {1,2}, {1,3}, {1,5}, {1,7}, {1,11} } };

  if (procno==sender) {
    comm_world.send( source_buffer.data(),indexed_where, receiver );
  } else if (procno==receiver) {
    auto recv_status =
      comm_world.recv( target_buffer.data(),fiveints, sender );
    int recv_count = recv_status.get_count<int>();
    assert(recv_count==count);
  }
  
  if (procno==receiver) {
    int i=3,val=7;
    if (target_buffer[i]!=val)
      printf("Error: location %d %d s/b %d\n",i,target_buffer[i],val);
    i=4; val=11;
    if (target_buffer[i]!=val)
      printf("Error: location %d %d s/b %d\n",i,target_buffer[i],val);
    printf("Finished. Correctly sent indexed primes.\n");
  }

  return 0;
}
