/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Programming for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2020
   %%%%
   %%%% lockall.c : passive target synchronization & atomic update
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

  MPI_Comm comm;
  int procno=-1,nprocs,ierr,err;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);

  int repo=0, master = nprocs-1;

  /*
   * Make a window that is only nonzero on procno==repo
   */
  int *win_buffer;
  MPI_Win the_window;
  MPI_Aint winsize = 0;sizeof(MPI_INT);
  if (procno==repo)
    winsize = sizeof(MPI_INT);
  MPI_Win_allocate( winsize, sizeof(MPI_INT), MPI_INFO_NULL, comm, &win_buffer, &the_window);
  MPI_Win_fence(the_window,0);

  /*
   * The master rank initializes the window
   */
  if (procno==master) {
    int zero=0;
    MPI_Aint zero_disp = 0;
    MPI_Put( &zero,1,MPI_INT, repo,zero_disp, 1,MPI_INT,
	     the_window);
  }
  MPI_Win_fence(the_window,0);

  /*
   * Passive target epoch
   */
  int assert = 0; // MPI_MODE_NOCHECK;
  MPI_Win_lock_all(assert, the_window);
  int readout;
  if (procno == master) {
    printf("Master is done!\n");
  } else {
    int one=1;
    //    MPI_Fetch_and_op(&one, &readout, MPI_INT, repo, 0, MPI_SUM, the_window);
  }
  if (procno==master)
    printf("Final window: readout=%d buffer=%d\n",readout,win_buffer[0]);
  MPI_Win_unlock_all(the_window);
  MPI_Win_free(&the_window);

  MPI_Finalize();
  return 0;
}
