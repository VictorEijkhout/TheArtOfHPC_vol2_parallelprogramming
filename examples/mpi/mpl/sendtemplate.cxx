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
 
template <typename T>
void send_and_recv( const mpl::communicator &comm, vector<T> &v );
int main() {
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  if (comm_world.size()<2)
    return EXIT_FAILURE;

  if (comm_world.rank()==0)
    cout << "testing with ints of size " << sizeof(long long) << endl;
  vector<long long> v(8);
  send_and_recv( comm_world,v );

  if (comm_world.rank()==0)
    cout << "testing with complex<float> " << sizeof(complex<float>) << endl;
#include <complex>
  vector<complex<float>> c(8);
  send_and_recv( comm_world,c );

  if (comm_world.rank()==0)
    cout << "testing with complex<double> " << sizeof(complex<double>) << endl;
  vector<complex<double>> d(8);
  send_and_recv( comm_world,d );

  return EXIT_SUCCESS;
}

template <typename T>
void send_and_recv( const mpl::communicator &comm, vector<T> &v ) {
  mpl::contiguous_layout<T> v_layout(v.size());

  // Initialize the data
  if (comm.rank()==0) {

    T init=0;
    for (T &x : v) {
      x=init;
      init += 1;
    }
    
    comm.send(v.data(), v_layout, 1); // send to rank 1
    cout << "sent: ";
    for (T &x : v) 
      cout << x << ' ';
    cout << '\n';

  } else if (comm.rank()==1) {

    comm.recv(v.data(), v_layout, 0);  // receive from rank 0

    cout << "got : ";
    for (T &x : v) 
      cout << x << ' ';
    cout << '\n';

  }
}

