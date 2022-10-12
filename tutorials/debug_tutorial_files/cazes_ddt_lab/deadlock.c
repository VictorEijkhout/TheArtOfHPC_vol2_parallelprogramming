/*
*  Example program that purposefully deadlocks MPI communication by using
*  blocking communication statements in the wrong order. From an original
*  example by John Cazes @ TACC.
*
*  Compile using: mpicc -g -O0 -o deadlock deadlock.c
*
*  v1.0 - 2009/03/12 - carlos@tacc.utexas.edu
*  v1.1 - 2018/06/27 - dmcdougall@tacc.utexas.edu
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  /* Local variables */
  int i, proc, nprocs;
  MPI_Status MPI_STAT;
  const int NMAX = 25000, TAG1 = 1, TAG2 = 2;
  int snd_buffer[NMAX], rcv_buffer[NMAX];

  /* Initialize MPI communication               *
   * Total number of processors is nprocs       *
   * Processor ranks range from 0 to (nprocs-1) */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc);

  /* Initialize data */
  for (i = 0; i < NMAX; i++) {
    if (proc == 0) {
      snd_buffer[i] = i + 1;
    }
    else {
      snd_buffer[i] = 2*(i + 1);
    }
    rcv_buffer[i] = 0;
  }

  /* Write some data to verify the data exchange later on */
  if (proc == 0)
    printf("\nP:%d - (%d,%d)", proc, snd_buffer[0], snd_buffer[NMAX-1]);
  if (proc == nprocs - 1)
    printf("\nP:%d - (%d,%d)", proc, snd_buffer[0], snd_buffer[NMAX-1]);


  /* Exchange data */
  for (i = 1; i < nprocs; i++) {

    /* Send information from processor 0 to processor i and collect information *
     * from processor i into processor 0 using blocking comms.                  */
    if (proc == 0) {
      MPI_Send(snd_buffer, NMAX, MPI_INT, i, TAG1, MPI_COMM_WORLD);
      MPI_Recv(rcv_buffer, NMAX, MPI_INT, i, TAG2, MPI_COMM_WORLD, &MPI_STAT);
    }

    /* Send information from processor i to processor 0 and collect information *
     * from processor 0 into processor i using blocking comms.                  */
    if (proc == i) {
      MPI_Send(snd_buffer, NMAX, MPI_INT, 0, TAG2, MPI_COMM_WORLD);
      MPI_Recv(rcv_buffer, NMAX, MPI_INT, 0, TAG1, MPI_COMM_WORLD, &MPI_STAT);
    }
  }

  /* Make sure all processes are synchronized */
  MPI_Barrier(MPI_COMM_WORLD);

  /* Write some data to verify the data exchange */
  if (proc == 0)
    printf("\nP:%d - (%d,%d)", proc, rcv_buffer[0], rcv_buffer[NMAX-1]);
  if (proc == nprocs - 1)
    printf("\nP:%d - (%d,%d)", proc, rcv_buffer[0], rcv_buffer[NMAX-1]);

  /* Finalize MPI and stop program */
  MPI_Finalize();

  return 0;
}
