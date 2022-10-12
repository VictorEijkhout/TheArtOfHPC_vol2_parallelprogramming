/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2021
   %%%%
   %%%% commsplit.cx : illustrating MPI_Comm_split in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
using std::endl;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world=mpl::environment::comm_world();
  auto procno = comm_world.rank();
  auto nprocs = comm_world.size();

  // create sub communicator modulo 2
  int color2 = procno % 2;
  mpl::communicator comm2( mpl::communicator::split, comm_world, color2 );
  auto procno2 = comm2.rank();

  // create sub communicator modulo 4 recursively
  int color4 = procno2 % 2;
  mpl::communicator comm4( mpl::communicator::split, comm2, color4 );
  auto procno4 = comm4.rank();

  int mod4ranks[nprocs];
  comm_world.gather( 0, procno4,mod4ranks );
  if (procno==0) {
    cout << "Ranks mod 4:";
    for (int ip=0; ip<nprocs; ip++)
      cout << " " << mod4ranks[ip];
    cout << endl;
  }

  if (procno/4!=procno4)
    printf("Error %d %d\n",procno,procno4);

  if (procno==0)
    printf("Finished\n");

  return 0;
}

