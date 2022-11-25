/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% darray.c : distributed array datatype
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

  MPI_Init(&argc,&argv);

  const int ndim = 2; int psizes[ndim];
  psizes[0] = 0; psizes[1] = 0;
  MPI_Dims_create( nprocs,ndim,psizes );

  int gsizes[ndim],distribs[ndim],dargs[ndim];
  memset(gsizes,  0,ndim*sizeof(int));
  memset(distribs,0,ndim*sizeof(int));
  memset(dargs,   0,ndim*sizeof(int));

  const int blowup = 10;
  for (int id=0; id<ndim; id++) {
    gsizes[id]   = psizes[id] * blowup;
    distribs[id] = MPI_DISTRIBUTE_BLOCK;
  }

  MPI_Datatype distrib_float;
  MPI_Type_create_darray
    ( nprocs,procno,
      ndim,
      gsizes,distribs,dargs,psizes,
      MPI_ORDER_C,
      MPI_FLOAT,
      &distrib_float);

  MPI_Finalize();
  return 0;
}
