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
#include <vector>
#include <cmath>
using namespace std;
#include <mpi.h>

#define max(a,b) ( (a)>(b) ? (a) : (b) )

int main(int argc,char **argv) {

  MPI_Comm comm;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  stringstream proctext;

  // first set a unique random seed
  srand((int)(procno*(double)RAND_MAX/nprocs));

  /*
   * How many elements do I have locally?
   * Allocate space.
   */
  int
    my_number_of_elements = rand() % nprocs,
    my_first_index=0;
  /*
   * Exercise:
   * - determine the index of your first element.
   *   Which routine do you use here?
   */
/**** your code here ****/
    (&my_number_of_elements,&my_first_index,
     1,MPI_INT,MPI_SUM,comm);
  printf("Proc %3d has %3d elements, range [%4d,%4d)\n",
         procno,my_number_of_elements,my_first_index,my_first_index+my_number_of_elements);

  /*
   * Create a local array of size `my_number_of_elements'
   * Fill in this local array
   */
  int total_number_of_elements;
  vector<int> my_elements( my_number_of_elements );
  for (int i_element=0; i_element<my_number_of_elements; i_element++)
    my_elements[i_element] = my_first_index+i_element;

  MPI_Reduce( &my_number_of_elements,&total_number_of_elements,
	      1,MPI_INT,MPI_SUM,0,comm);
  if (procno==0) {
    printf("Total number of elements: %d\n",total_number_of_elements);
  }

  /*
   * Gather all local arrays into one global
   */
  // how many elements from each process?
  vector<int> size_buffer;
  if (procno==0)
    size_buffer = vector<int>( nprocs );
  MPI_Gather
    (
/**** your code here ****/
     );
  // where are they going to go in the big buffer?
  vector<int> displ_buffer;
  if (procno==0)
    displ_buffer = vector<int>( nprocs );
  MPI_Gather
    (
/**** your code here ****/
     );
  // now create the big buffer
  vector<int> gather_buffer;
  if (procno==0)
    gather_buffer = vector<int>( total_number_of_elements );
  /*
   * Use Gatherv to collect the small buffers into a big one
   */
  MPI_Gatherv
    (
/**** your code here ****/
     );

  /*
   * Print the gathered material
   */
  if (procno==0) {
    printf("Gathered:");
    for (int i_element=0; i_element<total_number_of_elements; i_element++)
      printf(" %d",gather_buffer[i_element]);
    printf("\n");
  }

  MPI_Finalize();
  return 0;
}
