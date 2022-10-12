/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% struct.c : illustrating type struct and extent measuring
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  if (nprocs<2) {
    printf("This program needs at least two processes\n");
    return -1;
  }
  int sender = 0, receiver = 1, the_other = 1-procno;
  struct object {
    char c;
    double x[2];
    int i;
  };

  size_t size_of_struct = sizeof(struct object);
  if (procno==sender)
    printf("Structure has size %ld, naive size %ld\n",
	   size_of_struct,
	   sizeof(char)+2*sizeof(double)+sizeof(int));
  struct object myobject;
  if (procno==sender) {
    myobject.c = 'x';
    myobject.x[0] = 2.7; myobject.x[1] = 1.5;
    myobject.i = 37;
  }

  MPI_Datatype newstructuretype;
  int structlen = 3;
  int blocklengths[structlen]; MPI_Datatype types[structlen];
  MPI_Aint displacements[structlen];

  /*
   * where are the components relative to the structure?
   */
  MPI_Aint current_displacement=0;

  // one character
  blocklengths[0] = 1; types[0] = MPI_CHAR;
  displacements[0] = (size_t)&(myobject.c) - (size_t)&myobject;

  // two doubles
  blocklengths[1] = 2; types[1] = MPI_DOUBLE;
  displacements[1] = (size_t)&(myobject.x) - (size_t)&myobject;

  // one int
  blocklengths[2] = 1; types[2] = MPI_INT;
  displacements[2] = (size_t)&(myobject.i) - (size_t)&myobject;

  MPI_Type_create_struct(structlen,blocklengths,displacements,types,&newstructuretype);
  MPI_Type_commit(&newstructuretype);

  MPI_Aint typesize,typelb;
  MPI_Type_get_extent(newstructuretype,&typelb,&typesize);
  assert( typesize==size_of_struct );
  if (procno==sender) {
    printf("Type extent: %ld bytes; displacements: %ld %ld %ld\n",
	   typesize,displacements[0],displacements[1],displacements[2]);
  }
  if (procno==sender) {
    MPI_Send(&myobject,1,newstructuretype,the_other,0,comm);
  } else if (procno==receiver) {
    MPI_Recv(&myobject,1,newstructuretype,the_other,0,comm,MPI_STATUS_IGNORE);
  }
  MPI_Type_free(&newstructuretype);

  /* if (procno==sender) */
  /*   printf("char x=%ld, l=%ld; double x=%ld, l=%ld, int x=%ld, l=%ld\n", */
  /* 	   char_extent,char_lb,double_extent,double_lb,int_extent,int_lb); */
  
  if (procno==receiver) {
    printf("Char '%c' double0=%e double1=%e int=%d\n",
	   myobject.c,myobject.x[0],myobject.x[1],myobject.i);
    ASSERT(myobject.x[1]==1.5);
    ASSERT(myobject.i==37);
  }

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}

#if 0
  blocklengths[0] = 1; types[0] = MPI_CHAR;
  displacements[0] = (size_t)&(myobject.c) - (size_t)&myobject;
  blocklengths[1] = 2; types[1] = MPI_DOUBLE;
  displacements[1] = (size_t)&(myobject.x[0]) - (size_t)&myobject;
  blocklengths[2] = 1; types[2] = MPI_INT;
  displacements[2] = (size_t)&(myobject.i) - (size_t)&myobject;

  MPI_Aint char_extent,char_lb;
  MPI_Type_get_extent(MPI_CHAR,&char_lb,&char_extent);
  /* if (procno==0) */
  /*   printf("CHAR lb=%ld xt=%ld disp=%ld\n",char_lb,char_extent,current_displacement); */
  MPI_Aint double_extent,double_lb;
  MPI_Type_get_extent(MPI_DOUBLE,&double_lb,&double_extent);
  /* if (procno==0) */
  /*   printf("DOUBLE lb=%ld xt=%ld disp=%ld\n",double_lb,double_extent,current_displacement); */
  MPI_Aint int_extent,int_lb;
  MPI_Type_get_extent(MPI_INT,&int_lb,&int_extent);
  /* if (procno==0) */
  /*   printf("INT lb=%ld xt=%ld disp=%ld\n",int_lb,int_extent,current_displacement); */
#endif

