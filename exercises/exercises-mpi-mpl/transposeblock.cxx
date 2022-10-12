/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% MPI example for transposing an array, MPL version
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;
#include <mpl/mpl.hpp>

#define BLOCKSIZE 4

int main(int argc,char **argv) {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  // compute communicator rank and size
  nprocs = comm_world.size();
  procno = comm_world.rank();

  if (nprocs<2) {
    stringstream proctext;
    proctext << "This program needs at least two processes" << endl;
    cerr << proctext.str(); proctext.clear();
    return -1;
  }
  int sender = 0, localsize = 10;

  int blocksize_i = BLOCKSIZE, blocksize_j = BLOCKSIZE;

  int isize = nprocs * blocksize_i, jsize = nprocs * blocksize_j;
  vector<int>
    data_0( blocksize_j*isize ),
    data_1( blocksize_i*jsize );
  for (int col=0; col<blocksize_j; col++)
    for (int row=0; row<isize; row++)
      data_0[ col*isize + row ] =
	/* get to my column */ procno*isize*blocksize_j + col*isize +
	/* get to my row    */ row;

  {
    stringstream proctext;
    proctext << "Input data:\n";
    for (int row=0; row<isize; row++) {
      proctext << "Row " << setw(3) << row;
      for (int col=0; col<blocksize_j; col++)
	proctext << " " << setw(3) << data_0[ row + col*isize ];
      proctext << "\n";
    }
    cout << proctext.str();
  }

  /*
   * What are we sending to proc p? Block:
   * - starting at           : i=p*blocksize_i
   * - depth of block        : blocksize_i
   * - stride between blocks : isize
   * - how many blocks       : blocksize_j
   */
  mpl::strided_vector_layout<int>
    sourceblock( blocksize_j,blocksize_i,isize );

  /*
   * Each continguous source block becomes a strided column in the transpose:
   * - number of blocks      : blocksize_i
   * - size of block         : 1
   * - stride between blocks : jsize ( = line length of the transpose )
   */
  mpl::strided_vector_layout<int>
    targetcolumn( blocksize_i,1,jsize );

  /*
   * Exercise: 
   * - resize the targetcolumn type to make its extent smaller
   */
  auto  skinnycolumn(targetcolumn);
/**** your code here ****/

  /*
   * Exercise:
   * - make the `targetblock' type that packs a number of `skinnycolumn's together
   */
/**** your code here ****/

  /*
   * Every rank initiates a gather
   */
  stringstream proctext;
  for (int p=0; p<nprocs; p++) {
    comm_world.gather
      ( p,
        data_0.data()+p*blocksize_i,sourceblock,
        data_1.data(),targetblock );
    // MPI_Gather( data_0+p*blocksize_i,1,sourceblock,
    // 		data_1,              1,targetblock,
    // 		p,comm );
    if (p==procno) {
      proctext << "Output data:\n";
      for (int row=0; row<blocksize_i; row++) {
	proctext << "Row " << setw(3) << procno*blocksize_i+row << ":";
	for (int col=0; col<jsize; col++)
	  proctext << " " << setw(3) << data_1[ row*jsize + col ];
	proctext << "\n";
      }
    }
  }
  
  if (procno==0) {
    stringstream proctext;
    proctext << "Finished\n";
    cout << proctext.str();
  }

  return 0;
}
