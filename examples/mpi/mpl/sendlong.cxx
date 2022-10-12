/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2021
   %%%%
   %%%% sendlong long.cxx : send a buffer in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <iostream>
#include <vector>
#include <mpl/mpl.hpp>
using namespace std;
 
void send_and_recv( const mpl::communicator &comm, vector<long long> &v ) {
  mpl::contiguous_layout<long long> v_layout(v.size());

  // Initialize the data
  if (comm.rank()==0) {

    cout << "testing with ints of size " << sizeof(long long) << endl;
    long long init=0;
    for (long long &x : v) {
      x=init;
      ++init;
    }
    
    comm.send(v.data(), v_layout, 1); // send to rank 1
    cout << "sent: ";
    for (long long &x : v) 
      cout << x << ' ';
    cout << '\n';

  } else if (comm.rank()==1) {

    comm.recv(v.data(), v_layout, 0);  // receive from rank 0

    cout << "got : ";
    for (long long &x : v) 
      cout << x << ' ';
    cout << '\n';

  }
}

int main() {
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  if (comm_world.size()<2)
    return EXIT_FAILURE;

  vector<long long> v(8);
  send_and_recv( comm_world,v );

  return EXIT_SUCCESS;
}

