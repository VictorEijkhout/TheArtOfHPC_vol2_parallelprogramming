/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2018-2020
   %%%%
   %%%% ibarrierupdate.c : illustrating MPI_Ibarrier in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
using namespace std;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  // first set a unique random seed
  srand((int)(procno*(double)RAND_MAX/nprocs));

  /*
   * How many processes are we going to send to?
   */
  int n_destinations;
  n_destinations = nprocs * (float)rand() / (double)RAND_MAX;
  /*
   * Pick random processes to send to, and post an Isend
   */
  double send_data[n_destinations];
  mpl::irequest_pool send_requests;
  //  MPI_Request send_requests[n_destinations];
  for (int idestination=0; idestination<n_destinations; idestination++) {
    int destination;
    destination = nprocs * (float)rand() / (double)RAND_MAX;
    {
      stringstream proctext;
      proctext << "[" << procno << "] send to " << destination << endl;
      cerr << proctext.str(); proctext.clear();
    }
    send_requests.push
      ( comm_world.isend(send_data[idestination],destination) );
  }
  
  /*
   * When you're done sending, post a non-blocking barrier
   */
  {
    stringstream proctext;
    proctext << "[" << procno << "] posting barrier " << endl;
    cerr << proctext.str(); proctext.clear();
  }
  mpl::irequest final_barrier = comm_world.ibarrier();
  //  MPI_Ibarrier(comm,&final_barrier);

  int step=0;
  for ( ; ; step++) {
    /*
     * Exercise 1:
     * -- use MPI_Test to determine when the barrier is completed;
     * -- in that case you can quit
     */
    auto [all_done_flag,barrier_status] = final_barrier.test();
    //    MPI_Test(&final_barrier,&all_done_flag,MPI_STATUS_IGNORE);
/**** your code here ****/

    /*
     * Exercise part 2:
     * -- use MPI_Iprobe to test if there is a message
     * part 1: what do you do if there is no message?
     * part 2: the message can come from anywhere, so 
     *    you need to inspect the status to find the source and tag
     */
    MPI_Status recv_status;
    double receive_data;
    int flag;
    comm_world.iprobe( mpl::any_source,mpl::tag::any() );
    //    MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,comm,&flag,&recv_status);
/**** your code here ****/
    int source;
/**** your code here ****/
    comm_world.recv( receive_data,source );
    //    MPI_Recv(&receive_data,1,MPI_DOUBLE, source,MPI_ANY_TAG,comm,MPI_STATUS_IGNORE);
    {
      stringstream proctext;
      proctext << "[" << procno << "] received from " << source << endl;
      cerr << proctext.str(); proctext.clear();
    }
  }

  {
    stringstream proctext;
    proctext << "[" << procno << "] concluded after " << step << " steps" << endl;
    cerr << proctext.str(); proctext.clear();
  }
  final_barrier.wait();
  //  MPI_Wait(&final_barrier,MPI_STATUS_IGNORE);

  //  MPI_Finalize();
  return 0;
}
