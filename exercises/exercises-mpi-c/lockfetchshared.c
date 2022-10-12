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

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

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
  int assert = 0; // MPI_MODE_NOCHECK;
  MPI_Win_lock(MPI_LOCK_SHARED, repo, assert, the_window);
  if (procno == supervisor) {
    do {
      /*
       * Exercise: read out the windows content using an atomic operation
       */
/**** your code here ****/
      printf("Supervisor readout: %d\n", readout);
    } while( readout<nprocs-1 );
    printf("Supervisor is done!\n");
  } else {
    int one=1;
    MPI_Fetch_and_op(&one, &readout, MPI_INT, repo,zero_disp, MPI_SUM, the_window);
    MPI_Win_flush_local(repo,the_window);
    //MPI_Win_flush(repo,the_window);
    printf("[%d] adding 1 to %d\n",procno,readout);
  }
  MPI_Win_unlock(repo,the_window);
  if (procno==supervisor)
    printf("Supervisor window readout=%d\n",readout);
  if (procno==repo)
    printf("Window buffer=%d\n",win_buffer[0]);
  MPI_Win_free(&the_window);

  MPI_Finalize();
  return 0;
}
