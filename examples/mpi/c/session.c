/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2022
 ****
 **** MPI example for the session model
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  char *rank = getenv("PMI_RANK");
  //  printf("rank=%s\n",rank);
  int mainproc = !strcmp(rank,"0");

  MPI_Info session_request_info = MPI_INFO_NULL;
  MPI_Info_create(&session_request_info);
  char thread_key[] = "mpi_thread_support_level";
  MPI_Info_set(session_request_info,
               thread_key,"MPI_THREAD_MULTIPLE");

  MPI_Session the_session;
  MPI_Session_init
    ( session_request_info,MPI_ERRORS_ARE_FATAL,
      &the_session );

  MPI_Info session_actual_info;
  MPI_Session_get_info( the_session,&session_actual_info );
  char thread_level[100]; int info_len = 100, flag;
  MPI_Info_get_string( session_actual_info,
                       thread_key,&info_len,thread_level,&flag );
  if (flag) {
    if (mainproc) printf("Session has actual thread_level <<%s>>\n",thread_level);
  } else {
    if (mainproc) printf("Could not obtain thread level,flag=%d\n",flag);
  }

  // sanity check: do we find the world set?
  int world_pset=-1; char world_name[] = "mpi://WORLD";
  
  int npsets;
  MPI_Session_get_num_psets
    ( the_session,MPI_INFO_NULL,&npsets );
  if (mainproc) printf("Number of process sets: %d\n",npsets);
  for (int ipset=0; ipset<npsets; ipset++) {
    int len_pset; char name_pset[MPI_MAX_PSET_NAME_LEN];
    MPI_Session_get_nth_pset( the_session,MPI_INFO_NULL,
                              ipset,&len_pset,name_pset );
    if (mainproc)
      printf("Process set %2d: <<%s>>\n",ipset,name_pset);
    if (!strcmp(name_pset,world_name)) world_pset = ipset;
  }

  if (world_pset>=0) {
    if (mainproc) printf("Found WORLD as pset %d\n",world_pset);
  } else {
    if (mainproc) printf("Could not find WORLD pset. Fatal.\n"); return 1;
  }

  MPI_Group world_group = MPI_GROUP_NULL;
  MPI_Comm  world_comm  = MPI_COMM_NULL;
  MPI_Group_from_session_pset
    ( the_session,world_name,&world_group );
  MPI_Comm_create_from_group
    ( world_group,"victor-code-session.c",
      MPI_INFO_NULL,MPI_ERRORS_ARE_FATAL,
      &world_comm );
  MPI_Group_free( &world_group );
  int procid = -1, nprocs = 0;
  MPI_Comm_size(world_comm,&nprocs);
  MPI_Comm_rank(world_comm,&procid);
  if (procid==0)
    printf("World has %d processes\n",nprocs);

  MPI_Session_finalize( &the_session );

  return 0;
}
