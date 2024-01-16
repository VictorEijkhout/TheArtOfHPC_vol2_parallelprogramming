/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% MPI example for transposing an array
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define BLOCKSIZE 4

void print_array( MPI_Comm comm,int *data,char *id,int longside,int shortside ) {
  int procno;
  MPI_Comm_rank(comm,&procno);
  int summed_shortside;
  MPI_Reduce( &shortside,&summed_shortside,1,MPI_INT,MPI_SUM,0,comm );
  int *bigdata = NULL;
  if (procno==0)
    bigdata = (int*) malloc( longside*summed_shortside*sizeof(int) );
  MPI_Gather( data,longside*shortside,MPI_INT,
	      bigdata,longside*shortside,MPI_INT,
	      0,comm);
  if (procno==0) {
    printf("%s\n",id);
    for (int row=0; row<longside; row++) {
      printf("Row %3d:",row);
      for (int col=0; col<summed_shortside; col++)
	printf(" %3d",bigdata[ row + col*longside ]);
      printf("\n");
    }
  }
  if (procno==0)
    free(bigdata);
}

int main(int argc,char **argv) {

#define CHK(x) if (x) {						 \
    char errtxt[200]; int len=200;				 \
  MPI_Error_string(x,errtxt,&len);				 \
  printf("p=%d, line=%d, err=%d, %s\n",procno,__LINE__,x,errtxt); \
  return x;}

  MPI_Comm comm;
  int procno=-1,nprocs,ierr,err;
  err = MPI_Init(&argc,&argv); CHK(err);
  comm = MPI_COMM_WORLD;
  err = MPI_Comm_rank(comm,&procno); CHK(err);
  err = MPI_Comm_size(comm,&nprocs); CHK(err);
  err = MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN); CHK(err);

  int blocksize_i = BLOCKSIZE, blocksize_j = BLOCKSIZE;

  /*
   * Store a matrix in column-major:
   * the columns are contiguous
   * and contain successive numbers.
   */
  int isize = nprocs * blocksize_i, jsize = nprocs * blocksize_j;
  int
    *data_0 = (int*) malloc( blocksize_j*isize*sizeof(int) ),
    *data_1 = (int*) malloc( blocksize_i*jsize*sizeof(int) );
  for (int col=0; col<blocksize_j; col++)
    for (int row=0; row<isize; row++)
      data_0[ col*isize + row ] =
	/* get to my column */ procno*isize*blocksize_j + col*isize +
	/* get to my row    */ row;

  print_array( comm,data_0,"Input",isize,blocksize_j );

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
  for (int p=0; p<nprocs; p++) {
    MPI_Gather( data_0+p*blocksize_i,1,sourceblock,
		data_1,              1,targetblock,
		p,comm );
  }

  print_array( comm,data_1,"Output",jsize,blocksize_i );

  MPI_Type_free(&sourceblock)
  MPI_Type_free(&targetcolumn);
  MPI_Type_free(&skinnycolumn);
  MPI_Type_free(&targetblock);
  MPI_Finalize();

  free(data_0); free(data_1);
  if (procno==0)
    printf("Finished\n");

  return 0;
}
