/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-9
   %%%%
   %%%% intercomm.c : making an inter-communicator
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int main(int argc,char **argv) {

#include "globalinit.c"

  if (nprocs<4) {
    fprintf(stderr,"This program needs at least four processes\n");
    return -1;
  }
  if (nprocs%2>0) {
    fprintf(stderr,"This program needs an even number of processes\n");
    return -1;
  }

  int color,colors=2;
  MPI_Comm split_half_comm;

  int mydata = procno;
  // create sub communicator first & second half
  color = procno<nprocs/2 ? 0 : 1;
  int key=procno;
  if (color==0)
    // first half rotated
    key = ( procno+1 ) % (nprocs/2);
  else
    // second half numbered backwards
    key = nprocs-procno;
  MPI_Comm_split(MPI_COMM_WORLD,color,key,&split_half_comm);
  int sub_procno,sub_nprocs;
  MPI_Comm_rank(split_half_comm,&sub_procno);
  MPI_Comm_size(split_half_comm,&sub_nprocs);
  if (DEBUG) fprintf(stderr,"[%d] key=%d local rank: %d\n",procno,key,sub_procno);

  int
    local_leader_in_inter_comm
    = color==0 ? 2 : (sub_nprocs-2)
    ,
    local_number_of_other_leader
    = color==1 ? 2 : (sub_nprocs-2)
    ;

  if (local_leader_in_inter_comm<0 || local_leader_in_inter_comm>=sub_nprocs) {
    fprintf(stderr,
	    "[%d] invalid local member: %d\n",
	    procno,local_leader_in_inter_comm);
    MPI_Abort(2,comm);
  }
  int 
    global_rank_of_other_leader =
    1 + ( procno<nprocs/2 ? nprocs/2 : 0 )
    ;

  int
    i_am_local_leader = sub_procno==local_leader_in_inter_comm,
    inter_tag = 314;
  if (i_am_local_leader)
    fprintf(stderr,"[%d] creating intercomm with %d\n",
	   procno,global_rank_of_other_leader);
  MPI_Comm intercomm;
  MPI_Intercomm_create
    (/* local_comm:       */ split_half_comm,
     /* local_leader:     */ local_leader_in_inter_comm,
     /* peer_comm:        */ MPI_COMM_WORLD,
     /* remote_peer_rank: */ global_rank_of_other_leader,
     /* tag:              */ inter_tag,
     /* newintercomm:     */ &intercomm );
  if (DEBUG) fprintf(stderr,"[%d] intercomm created.\n",procno);
  
  if (i_am_local_leader) {
    int inter_rank,inter_size;
    MPI_Comm_size(intercomm,&inter_size);
    MPI_Comm_rank(intercomm,&inter_rank);
    if (DEBUG) fprintf(stderr,"[%d] inter rank/size: %d/%d\n",procno,inter_rank,inter_size);
  }

  double interdata=0.;
  if (i_am_local_leader) {
    if (color==0) {
      interdata = 1.2;
      int inter_target = local_number_of_other_leader;
      printf("[%d] sending interdata %e to %d\n",
	     procno,interdata,inter_target);
      MPI_Send(&interdata,1,MPI_DOUBLE,inter_target,0,intercomm);
    } else {
      MPI_Status status;
      MPI_Recv(&interdata,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,intercomm,&status);
      int inter_source = status.MPI_SOURCE;
      printf("[%d] received interdata %e from %d\n",
	     procno,interdata,inter_source);
      if (inter_source!=local_number_of_other_leader)
	fprintf(stderr,
		"Got inter communication from unexpected %d; s/b %d\n",
		inter_source,local_number_of_other_leader);
    }
  }
  
  int root; int bcast_data = procno;
  if (color==0) { // sending group: the local leader sends
    if (i_am_local_leader)
      root = MPI_ROOT;
    else
      root = MPI_PROC_NULL;
  } else { // receiving group: everyone indicates leader of other group
    root = local_number_of_other_leader;
  }
  if (DEBUG) fprintf(stderr,"[%d] using root value %d\n",procno,root);
  MPI_Bcast(&bcast_data,1,MPI_INT,root,intercomm);
  fprintf(stderr,"[%d] bcast data: %d\n",procno,bcast_data);

  if (procno==0)
    fprintf(stderr,"Finished\n");
 
  MPI_Finalize();
  return 0;
}
