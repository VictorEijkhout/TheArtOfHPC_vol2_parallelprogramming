/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** MPI Exercise to illustrate graph topologies
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  stringstream proctext;

  /*
   * Exercise:
   * -- declare the following parameters and set correct values:
   *    int nsources
   *    int sources[], destinations[], degrees[]
   * -- also declare, with values set to identically one:
   *    int weights[]
   */
  mpl::dist_graph_communicator::source_set ss;
  ss.insert( {procno,1} );
  if (procno>0)
    ss.insert( {procno-1,1} );
  mpl::dist_graph_communicator::dest_set ds;
  ds.insert( {procno,1} );
  if (procno<nprocs-1)
    ds.insert( {procno+1,1} );
  mpl::dist_graph_communicator pair_communicator( comm_world, ss,ds );
  {
    mpl::dist_graph_communicator::source_set
      ss = pair_communicator.inneighbors();
    auto // this is a dest_set
      ds = pair_communicator.outneighbors();
    stringstream proctext;
    proctext << "Proc " << procno << " has " << ss.size() << " sources: \n    ";
    for ( auto s : ss ) {
      proctext << s.first << ",";
    }
    proctext << "\n    and " << ds.size() << " destinations:\n    ";
    for ( auto d : ds ) {
      proctext << d.first << ",";
    }
    cout << proctext.str() << endl;
  }

  double mydata=procno, leftandme[2], leftdata;
#if 0
  MPI_Neighbor_allgather
    (&mydata,1,MPI_DOUBLE,
     leftandme,1,MPI_DOUBLE,
     pair_communicator);
  //printf("[%d] gathered: %5.3f %5.3f\n",procno,leftandme[0],leftandme[1]);
#endif
  cerr << "Call to all gather missing\n";
  leftdata = leftandme[0];

  /*
   * Check correctness
   */
  int
    error = procno>0 && leftdata!=mydata-1 ? procno : nprocs,
    errors = nprocs;
  comm_world.allreduce(mpl::min<int>(),error,errors);
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << endl;
    else
      proctext << "First error occurred on proc " << errors << endl;
    cerr << proctext.str(); proctext.clear();
  }

  return 0;
}
