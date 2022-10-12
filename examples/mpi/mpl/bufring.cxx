/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% bufring.cxx : buffered send in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <iostream>
#include <vector>
using std::vector;
#include <mpl/mpl.hpp>
 
int main(int argc,char **argv) {

  const mpl::communicator &comm_world=mpl::environment::comm_world();
  if (comm_world.size()<2)
    return EXIT_FAILURE;
  int nprocs = comm_world.size(), procno = comm_world.rank();

  int
    next = (procno+1) % nprocs,
    prev = (procno-1+nprocs) % nprocs;
  printf("[%d] to: %d, from: %d\n",procno,next,prev);

#define BUFLEN 10000

  vector<float> sbuf(BUFLEN), rbuf(BUFLEN);
  int size{ comm_world.bsend_size<float>(mpl::contiguous_layout<float>(BUFLEN)) };
  mpl::bsend_buffer buff(size);
  comm_world.bsend(sbuf.data(),mpl::contiguous_layout<float>(BUFLEN), next);

  printf("Send succeeded on %d\n",procno);
  comm_world.recv(rbuf.data(),mpl::contiguous_layout<float>(BUFLEN), prev);

  printf("Done on %d\n",procno);
  
  return 0;
}
