/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Programming for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2020-2022
   %%%%
   %%%% lockfetch.c : passive target synchronization & atomic update
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <mpi.h>

int main(int argc, char *argv[])
{

  MPI_Comm comm;
  int procno=-1,nprocs;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);

  // declare processes with special role
  int repo=0, supervisor = nprocs-1;

  /*
   * Make a window that is only nonzero on procno==repo
   */
  int *win_buffer;
  MPI_Win the_window;
  MPI_Aint zero_disp = 0;
  MPI_Aint winsize = 0;
  if (procno==repo)
    winsize = sizeof(MPI_INT);
  MPI_Win_allocate( winsize, sizeof(MPI_INT), MPI_INFO_NULL, comm, &win_buffer, &the_window);

  /*
   * The supervisor rank initializes the window
   */
  MPI_Win_fence(0,the_window);
  int workcount = 5 * nprocs;
  if (procno==supervisor) {
    MPI_Put( &workcount,1,MPI_INT, repo,zero_disp, 1,MPI_INT,
             the_window);
  }
  MPI_Win_fence(0,the_window);

  srand(procno);
  MPI_Win_lock
    (MPI_LOCK_SHARED,
     repo, 0, the_window);
  if (procno == supervisor) {
    /*
     * Supervisor cycles until the window is zero
     */
    int readout;
    do {
      /*
       * Exercise: merely reading the window is no longer enough!
       */
      int update=0;
      MPI_Fetch_and_op
        (&update, &readout ,
         MPI_INT,
         repo,zero_disp,
         MPI_NO_OP, the_window);
/**** your code here ****/
      printf("Supervisor readout: %d\n", readout);
    } while ( readout>0 );
    /*
     * Finish up: gather how much work every process has done
     * and correctness check
     */
    int nwork[nprocs], sum_work=0;
    MPI_Gather(&readout,1,MPI_INT,nwork,1,MPI_INT,supervisor,comm);
    nwork[supervisor] = 0;
    printf("Work by proc:");
    for (int ip=0; ip<nprocs; ip++) {
      sum_work += nwork[ip]; printf(" %d",nwork[ip]); }
    printf("\n");
    assert(sum_work==workcount);
  } else {
    /*
     * Worker processes do atomic update of the repo
     */
    int readout, my_work=0;
    do {
      float f = rand()/(float)RAND_MAX;
      if (f>.5) sleep(1);
      /*
       * Exercise: merely updating the window is not enough!
       */
      int mone = -1;
      MPI_Fetch_and_op(&mone, &readout, MPI_INT, repo,zero_disp, MPI_SUM, the_window);
      if (readout>0) my_work++;
      MPI_Win_flush_local
	(repo,the_window);
      printf("[%d] adding -1 to %d\n",procno,readout);
    } while (readout>0);
    // report how much work we have done
    MPI_Gather(&my_work,1,MPI_INT,NULL,1,MPI_INT,supervisor,comm);
  }
  MPI_Win_unlock(repo,the_window);

  MPI_Win_free(&the_window);

  MPI_Finalize();
  return 0;
}
