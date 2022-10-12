/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% portappmultiple.c : port stuff in a single executable
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <memory.h> // for strlen
#include <mpi.h>
#include <omp.h>

int main(int argc,char **argv) {

  int rlevel= MPI_THREAD_MULTIPLE, tlevel;
  MPI_Init_thread(&argc,&argv,rlevel,&tlevel);
  MPI_Comm
    comm_world = MPI_COMM_WORLD,
    comm_self = MPI_COMM_SELF;

  int world_p,world_n,work_n;
  MPI_Comm_size(comm_world,&world_n);
  work_n = world_n-1;
  MPI_Comm_rank(comm_world,&world_p);
  if (tlevel!=rlevel && world_p==0) {
    printf("No thread multiple support; only %d\n",tlevel);
    MPI_Abort(comm_world,0);
  }

  if (world_p==0) {

    /*
     * Manager creates in parallel an intercommunicator
     * for each of the workers.
     * This requires separate ports.
     */
    MPI_Comm intercomm[work_n];
#ifdef SEQ
    for (int worker=0; worker<work_n; worker++) {
#else
#pragma omp parallel num_threads(work_n)
    {
      int worker=omp_get_thread_num();
#endif
      int rank = worker+1;
      char myport[MPI_MAX_PORT_NAME];
      MPI_Open_port( MPI_INFO_NULL,myport );
      int portlen = strlen(myport);
      MPI_Send( myport,portlen+1,MPI_CHAR,
		/* add one to get world rank: */ rank,0,
		comm_world );
      printf("Host sent port <<%s>> to %d\n",myport,worker);
      MPI_Comm_accept( myport,MPI_INFO_NULL,0,comm_self,intercomm+worker );
      printf("host accepted connection\n");

      sleep(worker);
      int data = worker+1;
      MPI_Ssend( &data,1,MPI_INT,0,0,intercomm[worker] );
      MPI_Recv( &data,1,MPI_INT,0,0,intercomm[worker],MPI_STATUS_IGNORE );
      printf("... data returned from %d\n",worker);
      MPI_Close_port(myport);
    }
    for (int iwork=0; iwork<work_n; iwork++)
      MPI_Comm_free(intercomm+iwork);

  } else {

    int work_p = world_p-1;
    // for simplicity use comm self:
    MPI_Comm comm_work = MPI_COMM_SELF;
    /*
     * Worker accepts port and 
     * establishes inter communicator
     */
    char myport[MPI_MAX_PORT_NAME];
    MPI_Recv( myport,MPI_MAX_PORT_NAME,MPI_CHAR, 
	      MPI_ANY_SOURCE,0, comm_world,MPI_STATUS_IGNORE );
    printf("Worker %d received port <<%s>>\n",work_p,myport);
    MPI_Comm intercomm;
    MPI_Comm_connect( myport,MPI_INFO_NULL,0,comm_work,&intercomm );

    /*
     * Receive something from the manager thread
     */
    int value;
    fprintf(stderr, "Worker %d waiting for value...\n",world_p-1);
    MPI_Recv( &value, 1, MPI_INT,
	      /* from rank zero of manager comm */ 0,0,intercomm,MPI_STATUS_IGNORE );
    fprintf(stderr, "Worker %d received value %d from manager\n", 
	    world_p-1,value);
    MPI_Ssend( &value, 1, MPI_INT,
	      /* from rank zero of manager comm */ 0,0,intercomm );

    MPI_Close_port(myport);
    MPI_Comm_free(&intercomm);

  }

  MPI_Finalize();

  return 0;
}
