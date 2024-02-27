/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2024
 ****
 **** triangle.c : OpenMP collapse triangular iteration
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc,char **argv) {

  /*
   * Set problem size
   * Use a default or set as commandline argument
   */
  long int vectorsize = 100;
  if (argc==2)
    vectorsize = atoi(argv[1]);
  printf("Using vector size=%ld\n",vectorsize);

  double
    *particles = (double*)malloc(3*vectorsize*sizeof(double));
#define XCOORD(i) ((i)*3*vectorsize)
#define YCOORD(i) ((i)*3*vectorsize+1)
#define MASS(i) ((i)*3*vectorsize+2)
  for (int i=0; i<vectorsize; ++i) {
    particles[ XCOORD(i) ] = i;
    particles[ YCOORD(i) ] = vectorsize-i;
  }
  
  //codesnippet ompnbodycollapse
#pragma omp parallel for collapse(2)
  for ( int i=0; i<vectorsize; ++i ) {
    for ( int j=i+1; j<vectorsize; ++j ) {
      double
	dx= particles[XCOORD(i)]-particles[XCOORD(j)],
	dy= particles[YCOORD(i)]-particles[YCOORD(j)];
      double r = sqrt( dx*dx + dy*dy ), 
	f = (particles[MASS(i)]*particles[MASS(j)])/(r*r);
    }
  }
  //codesnippet end

  return 0;
}
