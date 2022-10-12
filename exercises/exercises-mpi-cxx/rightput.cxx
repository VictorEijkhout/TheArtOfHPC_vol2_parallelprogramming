/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise to illustrate one-sided
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  stringstream proctext;
  
  MPI_Init(&argc,&argv);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);


  double mydata=procno, leftdata=-2,
    window_data;

  // -- set `sendto' and `recvfrom'
  int sendto =
    ( procno<nprocs-1 ? procno+1 : MPI_PROC_NULL )
    ;

  MPI_Win the_window;
  MPI_Win_create(
/**** your code here ****/
		 MPI_INFO_NULL, comm,&the_window);

  // Exercise:
  // -- now do the Put call
  MPI_Win_fence(0,the_window);
/**** your code here ****/
  MPI_Win_fence(0,the_window);
  leftdata = window_data;


  int
    error = procno>0 && leftdata!=mydata-1 ? procno : nprocs,
    errors=-1;

  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << endl;
    else
      proctext << "First error occurred on proc " << errors << endl;
    cerr << proctext.str(); proctext.clear();
  }

  MPI_Win_free(&the_window);
  MPI_Finalize();

  return 0;
}
