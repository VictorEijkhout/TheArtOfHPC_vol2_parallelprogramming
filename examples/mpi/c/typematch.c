#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main() {

  MPI_Init(0,0);

  float x5;
  double x10;
  int s5,s10;
  MPI_Datatype mpi_x5,mpi_x10;
  
  MPI_Type_match_size(MPI_TYPECLASS_REAL,sizeof(x5),&mpi_x5);
  MPI_Type_match_size(MPI_TYPECLASS_REAL,sizeof(x10),&mpi_x10);
  MPI_Type_size(mpi_x5,&s5); 
  MPI_Type_size(mpi_x10,&s10);

  printf("%d, %d\n",s5,s10);

  MPI_Finalize();
  
  return 0;
}

