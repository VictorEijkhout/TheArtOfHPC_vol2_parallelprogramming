/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2021
   %%%%
   %%%% vector.cxx : vector datatype in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
using std::vector;
#include <cassert>

#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  if (nprocs<2) {
    printf("This program needs at least two processes\n");
    return -1;
  }
  int sender = 0, receiver = 1, the_other = 1-procno,
    count = 5,stride=2;
  vector<double>
    source(stride*count);
  vector<double>
    target(count);

  for (int i=0; i<stride*count; i++)
    source[i] = i+.5;

  if (procno==sender) {
    mpl::strided_vector_layout<double>
      newvectortype(count,1,stride);
    comm_world.send
      (source.data(),newvectortype,the_other);
  }
  else if (procno==receiver) {
    int recv_count;
    mpl::contiguous_layout<double> target_layout(count);
    mpl::status_t recv_status =
      comm_world.recv(target.data(),target_layout, the_other);
    recv_count = recv_status.get_count<double>();
    assert(recv_count==count);
  }
  
  if (procno==receiver) {
    for (int i=0; i<count; i++)
      if (target[i]!=source[stride*i])
	printf("location %d %e s/b %e\n",i,target[i],source[stride*i]);
  }

  if (procno==0)
    printf("Finished\n");

  return 0;
}
