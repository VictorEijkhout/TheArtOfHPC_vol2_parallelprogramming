/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise for the subarray type
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc,char **argv) {
  
  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;

  int nprocs, procno;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  /*
   * See if you can arrange the processes
   * in a perfect cube. If not, quit.
   */
  int procs_per_side;
  for (procs_per_side=1; ; procs_per_side++) {
    int scube = procs_per_side*procs_per_side*procs_per_side;
    /* if (procno==0) */
    /*   printf("testing s3=%d vs p=%d\n",scube,nprocs); */
    if (scube==nprocs)
      break;
    if (procs_per_side*procs_per_side*procs_per_side>nprocs) {
      if (procno==0)
	fprintf(stderr,"Number of processes needs to be a cube\n");
      fflush(stderr);
      MPI_Abort(comm,0);
    }
  }
  if (procno==0)
    printf("Using a cube of %d procs per side\n",procs_per_side);

  /*
   * Every process gets its own data and sends it to process zero.
   * Exercise: why are we using an Isend here?
   */
  int mydata = procno+1;
  MPI_Request send_request;
  MPI_Isend(&mydata,1,MPI_INT,0,0,comm,&send_request);
  
  /*
   * Now process zero receives all the contribution using the subarray type.
   * (You could use a gather here, but only because it is just one element)
   */
  if (procno==0) {
    int cubedata[procs_per_side*procs_per_side*procs_per_side],
      sides[3], sub_sizes[3], startpoint[3];
    for (int proci=0; proci<procs_per_side; proci++) {
      for (int procj=0; procj<procs_per_side; procj++) {
	for (int prock=0; prock<procs_per_side; prock++) {
	  /*
	   * Now receive the data, using a subarray type to
	   * put it in the right location of the cubedata buffer.
	   */
/**** your code here ****/
	  int sender = ( proci*procs_per_side + procj )*procs_per_side + prock;
	  MPI_Datatype insert_type;
	  MPI_Type_create_subarray
	    ( 3, sides,sub_sizes,startpoint,MPI_ORDER_C, MPI_INT,
	      &insert_type);
	  MPI_Type_commit(&insert_type);
	  /*
	   * Now do the receive call
	   */
	  MPI_Recv(
/**** your code here ****/
		   sender,0, comm,MPI_STATUS_IGNORE);
	  MPI_Type_free(&insert_type);
	}
      }
    }
    printf("Received cube:");
    int error=0;
    for (int p=0; p<nprocs; p++) {
      if (cubedata[p]!=p+1)
	error++;
      printf(" %d",cubedata[p]);
    }
    printf("\n");
    if (error==0)
      printf("Finished. Cube correctly gathered.\n");
    else
      printf("There were errors\n");
  }
  MPI_Wait(&send_request,MPI_STATUS_IGNORE);
  
  MPI_Finalize();
  return 0;
}
