/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2020
 ****
 **** isendandirecv.cxx : Simple illustration of send and recv in MPL
 ****
 ****************************************************************/

#include <cstdlib>
#include <complex>
#include <iostream>
using std::cout;
using std::endl;
#include <mpl/mpl.hpp>
 
int main(int argc,char **argv) {

  const mpl::communicator &comm_world=mpl::environment::comm_world();
  int
    nprocs = comm_world.size(),
    procno = comm_world.rank();
  if (comm_world.size()<2)
    return EXIT_FAILURE;

  int sender = 0,receiver = nprocs-1;

  if (procno==sender) {
    double send_data = 1.;
    mpl::irequest send_request
      ( comm_world.isend( send_data, receiver ) );
    send_request.wait();
    printf("[%d] Isend successfully concluded\n",procno);
  } else if (procno==receiver) {
    double recv_data;
    mpl::irequest recv_request =
      comm_world.irecv( recv_data,sender );
    recv_request.wait();
    printf("[%d] Ireceive successfully concluded\n",procno);
  }

  return 0;
}
