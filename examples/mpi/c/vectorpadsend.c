/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2021
   %%%%
   %%%% MPI example for resized vector type 
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm; int nprocs,procno;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  if (nprocs<2) {
    printf("Needs at least 2 processes\n");
    MPI_Abort(comm,0);
  }
  int sender=0, receiver=1;

  /*
   * Datatype for strided destinations
   */
  MPI_Datatype stridetype;
  int count = 3, stride = 2, blocklength = 1;
  int ntypes = 2, max_elements = ntypes*stride*count;
  if (procno==sender) {
    int *sendbuffer = (int*)malloc( max_elements*sizeof(int) );
    for (int i=0; i<max_elements; i++) sendbuffer[i] = i;
    MPI_Type_vector(count,blocklength,stride,MPI_INT,&stridetype);
    MPI_Type_commit(&stridetype);
    MPI_Send( sendbuffer,ntypes,stridetype, receiver,0, comm );
    free(sendbuffer);
  } else if (procno==receiver) {
    int *recvbuffer = (int*)malloc( max_elements*sizeof(int) );
    MPI_Status status;   
    MPI_Recv( recvbuffer,max_elements,MPI_INT, sender,0, comm,&status );
    int count; MPI_Get_count(&status,MPI_INT,&count);
    printf("Receive %d elements:",count);
    for (int i=0; i<count; i++) printf(" %d",recvbuffer[i]);
    printf("\n");
    free(recvbuffer);
  }
  // ntypes*stride

  MPI_Datatype paddedtype;
  if (procno==sender) {
    MPI_Aint l,e;
    int *sendbuffer = (int*)malloc( max_elements*sizeof(int) );
    for (int i=0; i<max_elements; i++) sendbuffer[i] = i;
    MPI_Type_get_extent(stridetype,&l,&e);
    printf("Stride type l=%ld e=%ld\n",l,e);
    e += ( stride-blocklength) * sizeof(int);
    MPI_Type_create_resized(stridetype,l,e,&paddedtype);
    MPI_Type_get_extent(paddedtype,&l,&e);
    printf("Padded type l=%ld e=%ld\n",l,e);
    MPI_Type_commit(&paddedtype);
    MPI_Send( sendbuffer,ntypes,paddedtype, receiver,0, comm );
    free(sendbuffer);
  } else if (procno==receiver) {
    int *recvbuffer = (int*)malloc( max_elements*sizeof(int) );
    MPI_Status status;   
    MPI_Recv( recvbuffer,max_elements,MPI_INT, sender,0, comm,&status );
    int count; MPI_Get_count(&status,MPI_INT,&count);
    printf("Receive %d elements:",count);
    for (int i=0; i<count; i++) printf(" %d",recvbuffer[i]);
    printf("\n");
    free(recvbuffer);
  }

  if (procno==sender) {
    MPI_Type_free(&paddedtype);
    MPI_Type_free(&stridetype);
  }

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
