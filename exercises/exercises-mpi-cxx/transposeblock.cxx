/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% MPI example for transposing an array, cxx version
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#include <mpi.h>

#define BLOCKSIZE 4

int main(int argc,char **argv) {

  MPI_Comm comm;
  int nprocs,procno;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  int blocksize_i = BLOCKSIZE, blocksize_j = BLOCKSIZE;

  int isize = nprocs * blocksize_i, jsize = nprocs * blocksize_j;
  int
    *data_0 = (int*) malloc( blocksize_j*isize*sizeof(int) ),
    *data_1 = (int*) malloc( blocksize_i*jsize*sizeof(int) );
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
  MPI_Datatype sourceblock;
  MPI_Type_vector( blocksize_j,blocksize_i,isize,MPI_INT,&sourceblock);
  MPI_Type_commit( &sourceblock);

  /*
   * Each continguous source block becomes a strided column in the transpose:
   * - number of blocks      : blocksize_i
   * - size of block         : 1
   * - stride between blocks : jsize ( = line length of the transpose )
   */
  MPI_Datatype targetcolumn;
  MPI_Type_vector( blocksize_i,1,jsize, MPI_INT,&targetcolumn);
  MPI_Type_commit( &targetcolumn );
  MPI_Datatype skinnycolumn;
  /*
   * Exercise: 
   * - resize the targetcolumn type to make its extent smaller
   */
  MPI_Type_create_resized( targetcolumn,0,
/**** your code here ****/
			   &skinnycolumn );
  MPI_Type_commit( &skinnycolumn );

  /*
   * Exercise:
   * - make the `targetblock' type that packs a number of `skinnycolumn's together
   */
  MPI_Datatype targetblock;
/**** your code here ****/
		       skinnycolumn, // old type
		       &targetblock  // new type
		       );
  MPI_Type_commit( &targetblock );

  /*
   * Every rank initiates a gather
   */
  stringstream proctext;
  for (int p=0; p<nprocs; p++) {
    MPI_Gather( data_0+p*blocksize_i,1,sourceblock,
		data_1,              1,targetblock,
		p,comm );
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
    proctext << "Finished\n";
    cout << proctext.str();
  }

  MPI_Finalize();
  return 0;
}
