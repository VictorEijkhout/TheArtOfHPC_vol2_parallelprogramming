/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% publishapp.c : publish stuff in a single executable
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <mpi.h>

int main(int argc,char **argv) {

  MPI_Init(&argc,&argv);
  MPI_Comm
    comm_world = MPI_COMM_WORLD,
    comm_self = MPI_COMM_SELF;

  int world_p,world_n;
  MPI_Comm_size(comm_world,&world_n);
  MPI_Comm_rank(comm_world,&world_p);

  /*
   * Set up a communicator for all the worker ranks
   */
  MPI_Comm comm_work;
  {
    MPI_Group group_world,group_work;
    MPI_Comm_group( comm_world,&group_world );
    int manager[] = {0};
    MPI_Group_excl( group_world,1,manager,&group_work );
    MPI_Comm_create( comm_world,group_work,&comm_work );
    MPI_Group_free( &group_world ); MPI_Group_free( &group_work );
  }

  char
    service_name[] = "exampleservice";
  if (world_p==0) {
    /*
     * On world process zero open a port, and
     * send its name to world process 1,
     * which is zero in the worker comm.
     */
    MPI_Comm intercomm;
    char myport[MPI_MAX_PORT_NAME];
    MPI_Open_port( MPI_INFO_NULL,myport );
    MPI_Publish_name( service_name, MPI_INFO_NULL, myport );
    MPI_Comm_accept( myport,MPI_INFO_NULL,0,comm_self,&intercomm );
    printf("Manager accepted connection on port <<%s>>\n",myport);

    /*
     * After the workers have accept the connection,
     * we can talk over the inter communicator
     */
    int work_n;
    MPI_Comm_remote_size(intercomm,&work_n);
    double work_data[work_n];
    MPI_Send( work_data,work_n,MPI_DOUBLE,
	      /* to rank zero of worker comm */ 0,0,intercomm );
    printf("Manager sent %d items over intercomm\n",work_n);
    /*
     * After we're done, close the port, and unpublish the service
     */
    MPI_Close_port(myport);
    MPI_Unpublish_name( service_name, MPI_INFO_NULL, myport );

  } else {

    /*
     * See if we can find the service
     */
    char myport[MPI_MAX_PORT_NAME];
    MPI_Lookup_name( service_name,MPI_INFO_NULL,myport );

    /*
     * The workers collective connect over the inter communicator
     */
    MPI_Comm intercomm;
    MPI_Comm_connect( myport,MPI_INFO_NULL,0,comm_work,&intercomm );

    int work_p,work_n;
    MPI_Comm_size( comm_work,&work_n );
    MPI_Comm_rank( comm_work,&work_p );

    if (work_p==0) {
      int manage_n;
      MPI_Comm_remote_size(intercomm,&manage_n);
      printf("%d workers connected to %d managers\n",work_n,manage_n);
    }

    /*
     * The local leader receives work from the manager
     */
    if (work_p==0) {
      double work_data[work_n];
      MPI_Status work_status;
      MPI_Recv( work_data,work_n,MPI_DOUBLE, 
		/* from rank zero of manager comm */ 0,0,intercomm,&work_status );
      int work_count;
      MPI_Get_count(&work_status,MPI_DOUBLE,&work_count);
      printf("Worker zero received %d data items from manager\n",work_count);
    }
    /*
     * After we're done, close the connection
     */
    MPI_Close_port(myport);
  }

  MPI_Finalize();

  return 0;
}
