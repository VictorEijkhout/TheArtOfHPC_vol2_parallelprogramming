/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-5
   %%%%
   %%%% tools.c : auxiliary routines
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "tools.h"

/**
   This routine prints an error message and returns a nonzero code.
   
   * id: the current processor prints a message if this is its Comm_rank, 
         or if id is negative
   * fmt: this routine accepts the same syntax as printf.
*/
int error(MPI_Comm comm,int id,const char *fmt,...)
{
  int procno; va_list argp;
  MPI_Comm_rank(comm,&procno);
  va_start(argp,fmt);
  if (id<0 || id==procno) {
    vprintf(fmt, argp); printf("\n");
  }
  va_end(argp);
  return 1;
}

/**
   Parse the commandline arguments for the presence of a particular key.
   Only integer values are supported.

   If the keyword is not found, the default value is return.
*/
int commandline_argument(MPI_Comm comm,
    int argc,char **argv,const char *keyword,int default_val)
{
  int ipos,return_val,procno;
  MPI_Comm_rank(comm,&procno);
  return_val = default_val;
  if (procno==0) {
    for (ipos=1; ipos<argc-1; ipos++) {
      if (!strcmp(argv[ipos],keyword)) {
	return_val = atoi(argv[ipos+1]);
      }
    }
  }
  MPI_Bcast(&return_val,1,MPI_INT,0,comm);
  return return_val;
}

