/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% filewrite.c : MPI IO example
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  MPI_File mpifile;
  MPI_File_open
    (comm,"filewrite.dat",
     MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,
     &mpifile);
  MPI_File_write_at
    (mpifile,/* offset: */ procno*sizeof(int),
     &procno,1, MPI_INT,MPI_STATUS_IGNORE);
  MPI_File_close(&mpifile);

  if (procno==0) {
    int ncorrect=0;
    FILE *f;
    f = fopen("filewrite.dat","r");
    for (int i=0; i<nprocs; i++) {
      int fromfile,success;
      success = fread(&fromfile,sizeof(int),1,f);
      if (success==EOF) {
        printf("Premature end of file\n"); break;
      }
      if (fromfile==i)
	ncorrect++;
      else
        printf("Error %d:%d\n",i,fromfile);
    }
    fclose(f);
    printf("Finished: all %d correct\n",ncorrect);
  }

  MPI_Finalize();
  return 0;
}
