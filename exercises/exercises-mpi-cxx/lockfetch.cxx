/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Programming for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% lockfetch.c : passive target synchronization & atomic update
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <sstream>
using namespace std;
#include "mpi.h"

int main(int argc, char *argv[])
{

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  stringstream proctext;
  
  MPI_Init(&argc,&argv);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  int repo=0, supervisor = nprocs-1;
  MPI_Aint zero_disp = 0;

  /*
   * Make a window that is only nonzero on procno==repo
   */
  int *win_buffer;
  MPI_Win the_window;
  MPI_Aint winsize = 0;
  if (procno==repo)
    winsize = sizeof(MPI_INT);
  MPI_Win_allocate( winsize, sizeof(MPI_INT), MPI_INFO_NULL, comm, &win_buffer, &the_window);

  /*
   * The supervisor rank initializes the window
   */
  MPI_Win_fence(0,the_window);
  if (procno==supervisor) {
    int zero=0;
    MPI_Put( &zero,1,MPI_INT, repo,zero_disp, 1,MPI_INT,
	     the_window);
  }
  MPI_Win_fence(0,the_window);

  /*
   * Passive target epoch
   */
  int readout;
  if (procno == supervisor) {
    do {
      int assert = 0; // MPI_MODE_NOCHECK;
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE, repo, assert, the_window);
      /*
       * Exercise: read out the window's content using an atomic operation
       */
/**** your code here ****/
      MPI_Win_unlock(repo,the_window);
      proctext << "Supervisor readout: " << readout << endl;
      cerr << proctext.str(); proctext.clear();
    } while( readout<nprocs-1 );
    proctext << "Supervisor is done!" << endl;
    cerr << proctext.str(); proctext.clear();
  } else {
    /*
     * Atomic update of the repo
     */
    int assert = 0; // MPI_MODE_NOCHECK;
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE, repo, assert, the_window);
    int one=1;
    MPI_Fetch_and_op(&one, &readout, MPI_INT, repo,zero_disp, MPI_SUM, the_window);
    MPI_Win_unlock(repo,the_window);
    proctext << "[" << procno << "] adding 1 to " << readout << endl;
    cerr << proctext.str(); proctext.clear();
  }
  if (procno==supervisor) {
    proctext << "Supervisor final readout " << readout << endl;
    cerr << proctext.str(); proctext.clear();
  }
  if (procno==repo) {
    proctext << "Window buffer=" << win_buffer[0] << endl;
    cerr << proctext.str(); proctext.clear();
  }
  MPI_Win_free(&the_window);

  MPI_Finalize();
  return 0;
}

