/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% spawnapp.c : spawning processes
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
    comm_self = MPI_COMM_SELF,
    comm_inter;
  int world_p,world_n;
  MPI_Comm_size(comm_world,&world_n);
  MPI_Comm_rank(comm_world,&world_p);

  int len = MPI_MAX_PROCESSOR_NAME;
  char procname[MPI_MAX_PROCESSOR_NAME]; 
  MPI_Get_processor_name(procname,&len);

  MPI_Comm comm_parent;
  MPI_Comm_get_parent(&comm_parent);
  int is_child = (comm_parent!=MPI_COMM_NULL);
  if (is_child) {
    int nworkers,workerno;
    MPI_Comm_size(MPI_COMM_WORLD,&nworkers);
    MPI_Comm_rank(MPI_COMM_WORLD,&workerno);
    printf("I detect I am worker %d/%d running on %s\n",
           workerno,nworkers,procname);

    int remotesize;
    MPI_Comm_remote_size(comm_parent, &remotesize);
    if (workerno==0) {
      printf("Worker deduces %d workers and %d parents\n",nworkers,remotesize);
    }

  } else {
    /*
     * Detect how many workers we can spawn
     */
    int universe_size, *universe_size_attr,uflag;
    MPI_Comm_get_attr
      (comm_world,MPI_UNIVERSE_SIZE,
       &universe_size_attr,&uflag);
    universe_size = *universe_size_attr;
    if (!uflag) universe_size = world_n;
    int work_n = universe_size - world_n;
    if (world_p==0) {
      printf("A universe of size %d leaves room for %d workers\n",
             universe_size,work_n);
      printf(".. spawning from %s\n",procname);
    }

    if (work_n<=0) 
      MPI_Abort(comm_world,1);
    const char *workerprogram = "./spawnapp";
    MPI_Comm_spawn(workerprogram,MPI_ARGV_NULL,
                   work_n,MPI_INFO_NULL,
                   0,comm_world,&comm_inter,NULL);
  }

  MPI_Finalize();

  return 0;
}

