/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise to illustrate graph topologies
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Init(&argc,&argv);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);


  /*
   * Exercise:
   * -- declare the following parameters and set correct values:
   *    int nsources
   *    int sources[], destinations[], degrees[]
   * -- also declare, with values set to identically one:
   *    int weights[]
   */
/**** your code here ****/
  MPI_Comm pair_communicator;
  MPI_Dist_graph_create
    (comm,
     nsources,     // number of source nodes declared here
     sources,      // where does our data come from?
     degrees,      // degrees of the sources
     destinations, // where does our data go to?
     weights,MPI_INFO_NULL,0,
     &pair_communicator
     );

  if (0) {
    int pair_nprocs,pair_procno;
    stringstream proctext;
    MPI_Comm_size(pair_communicator,&pair_nprocs);
    MPI_Comm_rank(pair_communicator,&pair_procno);
    proctext << "[" << procno << "] rank in pair comm: "
	     << pair_procno << "/" << pair_nprocs << "\n";
    cerr << proctext.str();
  }

  double mydata=procno, leftandme[2], leftdata;
  MPI_Neighbor_allgather
    (&mydata,1,MPI_DOUBLE,
     leftandme,1,MPI_DOUBLE,
     pair_communicator);
  //printf("[%d] gathered: %5.3f %5.3f\n",procno,leftandme[0],leftandme[1]);
  leftdata = leftandme[0];



  int
    error = procno>0 && leftdata!=mydata-1 ? procno : nprocs,
    errors=-1;

  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    stringstream proctext;
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << "\n";
    else
      proctext << "First error occurred on proc " << errors << "\n";
    cerr << proctext.str();
  }

  MPI_Finalize();
  return 0;
}
