/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2025
 ****
 **** MPL layouts sending more than 2^31
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <mpi.h>
using namespace std;
#include <mpl/mpl.hpp>

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs, procno;
  
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  int processA,processB;
  processA = 0; processB = nprocs-1;
  for ( size_t s0=28; s0<38; s0++ ) {
    stringstream proctext;
    //codesnippet mplbigbuffer
    size_t s;
    //codesnippet end
    s = static_cast<size_t>(1)<<(s0-1);
    if ( procno==processA or procno==processB ) {
      if ( procno==processA )
        cout 
          << s0 << ": "
          << "max: " << vector<char>().max_size()
          << " vs actual: " << s << '\n';
      //codesnippet mplbigbuffer
      vector<char> buffer(s);
      mpl::contiguous_layout<char> buffersize(s);
      //codesnippet end
      if (procno==processA) {
        double t = MPI_Wtime();
        //codesnippet mplbigbuffer
        comm_world.send( buffer.data(),buffersize,processB );
        //codesnippet end
        comm_world.recv( buffer.data(),buffersize,processB );
        t = MPI_Wtime()-t; 
        {
          int nanosec = t*1000000000;
          proctext << "Time for pingpong of size 2^" << s0 << ": " 
                   << t
            //                   << fixed << setprecision(3) << nanosec*1.e-3 << " (microsec)"
            ;
        }
        cerr << proctext.str() << '\n';
      } else if (procno==processB) {
        comm_world.recv( buffer.data(),buffersize,processA );
        comm_world.send( buffer.data(),buffersize,processA );
      }
    }
  }

  return 0;
}
