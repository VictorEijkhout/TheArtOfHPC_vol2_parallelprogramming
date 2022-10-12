/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% struct.cxx : illustrating type struct in MPL
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
  int sender = 0, receiver = 1, the_other = 1-procno;
  char c; vector<double> x(2); int i;
  if (procno==sender) {
    c = 'x'; x[0] = 2.7; x[1] = 1.5; i = 37; }
  mpl::heterogeneous_layout object
    ( c,
      mpl::make_absolute(x.data(),mpl::vector_layout<double>(2)),
      i );
  if (procno==sender) {
    comm_world.send( mpl::absolute,object,receiver );
  } else if (procno==receiver) {
    comm_world.recv( mpl::absolute,object,sender );
  }

  if (procno==receiver) {
    printf("Char '%c' double0=%e double1=%e int=%d\n",
	   c,x[0],x[1],i);
    assert(x[1]==1.5);
    assert(i==37);
  }

  if (procno==0)
    printf("Finished\n");

  return 0;
}

