/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2023
   %%%%
   %%%% sendobject.cxx : send an iteratable object in MPL
   %%%% DOES NOT WORK
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <cstdio>
#include <complex>
#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <mpl/mpl.hpp>
 
class bag {
private:
  vector<int> contents;
public:
  bag(int n) : contents( vector<int>(n) ) {
    fprintf(stderr,"create size %d\n",n);
  };
  class iter {
  private: 
    int *ptr; int len;
  public:
    iter( int *ptr,int len=0 ) : ptr(ptr),len(len) {
      fprintf(stderr,"iter from addr %lu\n",(long)ptr);
    };
    void operator++() { ptr++; };
    bool operator!=(const bag::iter &other) { return ptr<other.ptr; };
    int& operator*() { return *ptr; }; // note reference!
  };
  bag::iter begin() {
    return iter( &(*contents.begin()), contents.size() ); };
  bag::iter end()   {
    return iter( &(*contents.end()) ); };
};

int main() {
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  if (comm_world.size()<2)
    return EXIT_FAILURE;
  int
    nprocs = comm_world.rank(),
    procno = comm_world.size();

  const int sender = 0, receiver = 1;
  if (procno==sender) {
    bag ten(10);
    cout << "Data:";
    for ( auto i : ten )
      cout << " " << i << '\n';
    cout << '\n';

    /*
     * Send and report
     */
    //codesnippet mplsendobject
    mpl::iterator_layout<int> in_ten( ten.begin(), ten.end() );
    comm_world.send( &(*ten.begin()),in_ten,receiver );  // send to rank 1
    //codesnippet end

  } else if (comm_world.rank()==receiver) {

    vector<double> v(10);

    /*
     * Receive data and report
     */
    comm_world.recv(v.begin(), v.end(), sender);  // receive from rank 0

    cout << "Got:";
    for ( auto x : v )
      cout << " " << x;
    cout << endl;
  }
  return EXIT_SUCCESS;
}

