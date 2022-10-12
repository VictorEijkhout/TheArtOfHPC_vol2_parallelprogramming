/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** MPI Exercise for the use of Scan/Exscan
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
using namespace std;
#include <mpl/mpl.hpp>

#define max(a,b) ( (a)>(b) ? (a) : (b) )

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  // set the default random number generator
  random_device r;
  default_random_engine generator{ r() };
  // distribution: int between 0 and nprocs
  uniform_int_distribution<int> distribution(0,nprocs);

  /*
   * How many elements do I have locally?
   * Allocate space.
   */
  int
    my_number_of_elements = distribution(generator),
    my_first_index=0;
  /*
   * Exercise:
   * - determine the index of your first element.
   *   Which routine do you use here?
   */
/**** your code here ****/
    (mpl::plus<int>(),
     my_number_of_elements,my_first_index);
  {
    stringstream proctext;
    proctext << "Proc " << setw(3) << procno
             << " has " << setw(3) << my_number_of_elements
             << " elements, range [" << setw(4) << my_first_index
             << "," << setw(4) << my_first_index+my_number_of_elements
             << ")\n";
    cout << proctext.str();
  }
  
  /*
   * Create a local array of size `my_number_of_elements'
   * Fill in this local array
   */
  int total_number_of_elements;
  vector<int> my_elements( max(my_number_of_elements,1) );
  for (int i_element=0; i_element<my_number_of_elements; i_element++)
    my_elements.at(i_element) = my_first_index+i_element;

  if (procno==0) {
    comm_world.reduce
      ( mpl::plus<int>(),0,my_number_of_elements,total_number_of_elements );
    stringstream proctext;
    proctext << "Total number of elements: " << total_number_of_elements;
    cout << proctext.str() << endl;
  } else {
    comm_world.reduce
      ( mpl::plus<int>(),0,my_number_of_elements );
  }

  /*
   * Use Gatherv to collect the small buffers into a big one
   */
  if (procno==0) {
    vector<int> size_buffer(nprocs);
    comm_world.gather
      (
/**** your code here ****/
       );
    {
      stringstream proctext;
      proctext << "Number of elements:";
      for ( int ip=0; ip<nprocs; ip++ )
	proctext << " " << size_buffer.at(ip);
      cout << proctext.str() << endl;
    }

    // now create the big buffer
    vector<int> gather_buffer( total_number_of_elements );

    mpl::layouts<int> receive_layout;
    for ( int iproc=0,loc=0; iproc<nprocs; iproc++ ) {
      auto siz = size_buffer.at(iproc);
      receive_layout.push_back
	( mpl::indexed_layout<int>( {{ siz,loc }} ) );
      loc += siz;
    }

    comm_world.gatherv
      (
/**** your code here ****/
       );

    /*
     * Print the gathered material
     */
    {
      stringstream proctext;
      proctext << "Gathered:";
      for (int i_element=0; i_element<total_number_of_elements; i_element++)
	proctext << " " << gather_buffer[i_element];
      cout << proctext.str() << endl;
    }

  } else {
    /*
     * If you are not the root, do versions with only send buffers
     */
    comm_world.gather
      ( 0,my_number_of_elements );

    comm_world.gatherv
      ( 0,my_elements.data(),mpl::contiguous_layout<int>(my_number_of_elements) );
  }

  return 0;
}
