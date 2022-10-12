/****************************************************************
 ****************************************************************
 ****
 **** This program file is part of the book and course
 **** "Parallel Computing"
 **** by Victor Eijkhout, copyright 2013-6
 ****
 **** spawn_worker.c : worker code for spawn example
 ****
 ****************************************************************
 ****************************************************************
 */


#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{

#define ASSERT(p) if (!(p)) {printf("Assertion failed for proc %d at line %d\n",procno,__LINE__); return -1;}
#define ASSERTm(p,m) if (!(p)) {printf("Message<<%s>> for proc %d at line %d\n",m,procno,__LINE__); return -1;}

  MPI_Comm comm;
  int procno=-1,nprocs,err;
  MPI_Init(&argc,&argv); 
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);

  int remotesize,nworkers,workerno;
  MPI_Comm parent;

  MPI_Comm_size(MPI_COMM_WORLD,&nworkers);
  MPI_Comm_rank(MPI_COMM_WORLD,&workerno);
  MPI_Comm_get_parent(&parent);
  ASSERTm(parent!=MPI_COMM_NULL,"No parent!");

  /*
   * To investigate process placement, get host name
   */
  {
    int namelen = MPI_MAX_PROCESSOR_NAME;
    char procname[namelen];
    MPI_Get_processor_name(procname,&namelen);
    printf("[%d] worker process runs on <<%s>>\n",workerno,procname);
  }

  /* 
   * Parallel code here.  
   * The manager is represented as the process with rank 0 in (the remote 
   * group of) MPI_COMM_PARENT.  If the workers need to communicate among 
   * themselves, they can use MPI_COMM_WORLD. 
   */

  char hostname[256]; int namelen = 256;
  MPI_Get_processor_name(hostname,&namelen);
  printf("worker %d running on %s\n",workerno,hostname);

  MPI_Finalize();
  return 0;
}
