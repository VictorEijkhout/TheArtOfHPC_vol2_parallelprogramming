/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2022
 ****
 **** MPI example for the session model;, multiple sessions
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Info info1 = MPI_INFO_NULL, info2 = MPI_INFO_NULL;
  char thread_key[] = "mpi_thread_support_level";
  MPI_Info_create(&info1); MPI_Info_create(&info2);
  MPI_Info_set(info1,thread_key,"MPI_THREAD_SINGLE");
  MPI_Info_set(info2,thread_key,"MPI_THREAD_MULTIPLE");
  MPI_Session session1,session2;
  MPI_Session_init( info1,MPI_ERRORS_ARE_FATAL,&session1 );
  MPI_Session_init( info2,MPI_ERRORS_ARE_FATAL,&session2 );

  char self_name[] = "mpi://SELF";
  MPI_Group self_group1,self_group2;
  MPI_Group_from_session_pset
    ( session1,self_name,&self_group1 );
  MPI_Group_from_session_pset
    ( session2,self_name,&self_group2 );
  MPI_Comm  self_comm1,self_comm2;
  MPI_Comm_create_from_group
    ( self_group1,"self-session1",
      MPI_INFO_NULL,MPI_ERRORS_ARE_FATAL,
      &self_comm1 );
  MPI_Comm_create_from_group
    ( self_group2,"self-session2",
      MPI_INFO_NULL,MPI_ERRORS_ARE_FATAL,
      &self_comm2 );
  MPI_Group_free( &self_group1 );
  MPI_Group_free( &self_group2 );

  /*
   * Not allowed to do this!
   */
  int same=0;
  MPI_Comm_compare(self_comm1,self_comm2,&same);
  printf("Self communicators compare as: %s\n",
	 same==MPI_IDENT ? "identical"
	 : same==MPI_CONGRUENT ? "similar"
	 : "different" );

  MPI_Session_finalize( &session1 );
  MPI_Session_finalize( &session2 );

  return 0;
}
