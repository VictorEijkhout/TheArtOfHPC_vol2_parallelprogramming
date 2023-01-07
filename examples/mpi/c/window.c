/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% window.c : aux stuff for windows
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include "mpi.h"
void test_window( MPI_Win the_window,MPI_Comm comm ) {
  int procno;
  MPI_Comm_rank(comm,&procno);
  //codesnippet winmodel
  int *modelstar,flag;
  MPI_Win_get_attr(the_window,MPI_WIN_MODEL,&modelstar,&flag);
  int model = *modelstar;
  if (procno==0)
    printf("Window model is unified: %d\n",model==MPI_WIN_UNIFIED);
  //codesnippet end
  if (!flag)
    printf("Could not get window model on p=%d\n",procno);
  int models[2]={model,-model},agree[2];
  MPI_Allreduce(models,agree,2,MPI_INT,MPI_MAX,comm);
  if (agree[0] == -agree[1]) {
    if (procno==0)
      printf("Window model is unified: %d\n",model==MPI_WIN_UNIFIED);
  } else {
    if (procno==0)
      printf("Memory models do not agree\n");
    printf("Window model on proc %d: %d\n",procno,model);
  }
}
