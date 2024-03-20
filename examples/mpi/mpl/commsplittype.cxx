/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% commsplittype.c : illustrating MPI_Comm_split in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
using std::endl;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &world_comm=mpl::environment::comm_world();
  auto procno = world_comm.rank();
  auto nprocs = world_comm.size();

  //codesnippet commsplittypempl
  mpl::communicator shared_comm
    ( mpl::communicator::split_shared_memory, world_comm );
  int
    onnode_procno = shared_comm.rank(),
    onnode_nprocs = shared_comm.size();
  //codesnippet end

  if (onnode_procno==0) {
  cout << "WE NEED TO REPORT PROC NAME\n";
   //  char procname[MPI_MAX_PROCESSOR_NAME]; int namlen;
   //  MPI_Get_processor_name(procname,&namlen);
   //  printf("[%d] is processor %d in a shared group of %d\n     running on %s\n",
   // procno,onnode_procno,onnode_nprocs,procname);
  }

  return 0;
}

