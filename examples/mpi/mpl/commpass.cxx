/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% commpass.c : passing communicator to subprogram
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

#include <mpl/mpl.hpp>
 
// BAD! this does a MPI_Comm_dup.
void comm_val( const mpl::communicator comm );

// correct!
void comm_ref( const mpl::communicator &comm );

void comm_val( const mpl::communicator comm ) {
  const mpl::communicator &world =
    mpl::environment::comm_world();
  
  cout << "value: " << boolalpha << (comm==world) << endl;
};

void comm_ref( const mpl::communicator &comm ) {
  const mpl::communicator &world =
    mpl::environment::comm_world();
  
  cout << "refer: " << boolalpha << (comm==world) << endl;
};

int main() {

  const mpl::communicator &comm =
    mpl::environment::comm_world();
  comm_val(comm);
  comm_ref(comm);

  return EXIT_SUCCESS;
}

