/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% portappmultiple.c : port stuff in a single executable
   %%%%
   %%%% user code with pthreads
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>
#include <mpi.h>


typedef struct {
  char port_name[MPI_MAX_PORT_NAME];
  pthread_t thread;
  MPI_Comm inter_comm;
  int rank;
} Client;


void* serverThreadFn(void *v) {
  Client *client = (Client*) v;

  // make sure the main thread has time to start MPI_Comm_accept again
    sleep(1);

  int value = 42;
  fprintf(stderr, "Manager sending value %d over intercomm...\n", value);
  MPI_Ssend( &value, 1, MPI_INT,
            /* to rank zero of worker comm */ 0,0,client->inter_comm );
  fprintf(stderr, "Manager sent value %d over client->inter_comm to %d\n",
	  value,client->rank);

  MPI_Recv(&value, 1, MPI_INT, 0, 0, client->inter_comm, MPI_STATUS_IGNORE);
  fprintf(stderr, "Manager received value %d over client->inter_comm from %d\n",
	  value,client->rank);

  MPI_Comm_disconnect(&client->inter_comm);

  return 0;
}


int main(int argc,char **argv) {

  int rlevel= MPI_THREAD_MULTIPLE, tlevel;
  MPI_Init_thread(&argc,&argv,rlevel,&tlevel);
  MPI_Comm
    comm_world = MPI_COMM_WORLD,
    comm_self = MPI_COMM_SELF;

  int world_p,world_n;
  MPI_Comm_size(comm_world,&world_n);
  MPI_Comm_rank(comm_world,&world_p);
  if (tlevel!=rlevel && world_p==0) {
    printf("No thread multiple support; only %d\n",tlevel);
    MPI_Abort(comm_world,0);
  }

  if (world_p==0) {
    int work_n = world_n-1;
    Client *clients = (Client*) malloc( work_n * sizeof(Client) );
    for (int iworker=0,rank=1; rank<world_n; iworker++,rank++ ) {

      Client *client = clients+iworker; // (Client*) malloc(sizeof(Client));
      client->rank = rank;

      fprintf(stderr, "Host establishing port to %d\n",rank);
      MPI_Open_port( MPI_INFO_NULL, client->port_name );
      int portlen = strlen(client->port_name);
      MPI_Ssend( client->port_name,portlen+1,MPI_CHAR,rank,0,comm_world );

      MPI_Comm_accept( client->port_name,MPI_INFO_NULL,0,comm_self,&client->inter_comm );
      fprintf(stderr, "host accepted connection from rank %d\n",rank);

      pthread_t thread;
      pthread_create(&thread, 0, serverThreadFn, client);
      client->thread = thread;

      pthread_join(client->thread, 0);
    }
    for (int iworker=0,rank=1; rank<world_n; iworker++,rank++ ) {
      Client *client = clients+iworker;
      //      pthread_join(clients->thread, 0);
      MPI_Close_port(client->port_name);
    }

  } else {

    MPI_Comm comm_work = MPI_COMM_SELF;

    printf("Worker %d waiting for port\n",world_p);
    char myport[MPI_MAX_PORT_NAME];
    MPI_Recv( myport,MPI_MAX_PORT_NAME,MPI_CHAR, 
	      MPI_ANY_SOURCE,0, comm_world,MPI_STATUS_IGNORE );
    fprintf(stderr,"Worker %d received port <<%s>>\n",world_p,myport);

    /*
     * The worker connects over the inter communicator
     */
    MPI_Comm intercomm;
    MPI_Comm_connect( myport,MPI_INFO_NULL,0,comm_work,&intercomm );

    /*
     * The local leader receives value from the manager
     */
    int value;
    fprintf(stderr, "Worker %d has intercomm, now waiting for value...\n",world_p);
    MPI_Recv( &value, 1, MPI_INT,
	      /* from rank zero of manager comm */ 0,0,intercomm,
	      MPI_STATUS_IGNORE );
    fprintf(stderr, "Worker %d received value %d from manager\n", 
	    world_p,value);
    value *= 10;
    MPI_Ssend(&value, 1, MPI_INT, 0, 0, intercomm);

    /*
     * After we're done, close the connection
     */
    MPI_Comm_free(&intercomm); // Comm_disconnect doesn't work either
  }

  MPI_Finalize();

  return 0;
}
