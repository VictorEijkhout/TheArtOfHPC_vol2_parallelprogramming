/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2024
   %%%%
   %%%% ijwrongright.c : show a common misconception in C
   %%%%
   %%%% NOTE for this example to go actually wrong, 
   %%%% compile with OPTLEVEL=0
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

#define INDEX(i,j) (i)*matrix_size+(j)
void matinit( double *matrix,int matrix_size ) {
  for ( int i=0; i<matrix_size; ++i )
    for ( int j=0; j<matrix_size; ++j )
      matrix[INDEX(i,j)] = 1./ (i+j+1);
}
double matsum( double *matrix,int matrix_size ) {
  double s;
  for ( int i=0; i<matrix_size; ++i )
    for ( int j=0; j<matrix_size; ++j )
      s += matrix[INDEX(i,j)];
  return s;
}
void noopt( double *matrix,int matrix_size ) {
  double s = matsum(matrix,matrix_size);
  if (s==0.) printf("%e\n",s);
}


int main(int argc,char **argv) {

  int matrix_size;
  if (argc==1)
    matrix_size = 100;
  else
    matrix_size = atoi(argv[1]);

  double *matrix = (double*)malloc( (long)matrix_size*matrix_size*sizeof(double) );
  if (!matrix) {
    printf("Could not allocate\n"); return 1; }

  double sum;
  double tstart,duration;
  int nex;
  /*
   * Sequential for reference
   */
  matinit( matrix,matrix_size );
  tstart = omp_get_wtime();
  for ( nex=0; nex<100; ++nex) {
    int i,j;
    for ( i=0; i<matrix_size; ++i )
      for ( j=0; j<matrix_size; ++j )
        matrix[INDEX(i,j)] *= 3.14;
  }
  duration = omp_get_wtime()-tstart;
  sum = matsum( matrix,matrix_size );
  printf("Sequential time=%e, sum=%e\n",duration,sum);

  /*
   * The wrong way
   */
  matinit( matrix,matrix_size );
  tstart = omp_get_wtime();
  for ( nex=0; nex<100; ++nex) {
    //codesnippet ijwrongshare
    int i,j;
    #pragma omp parallel for
    for ( i=0; i<matrix_size; ++i )
      for ( j=0; j<matrix_size; ++j )
        matrix[INDEX(i,j)] *= 3.14;
    //codesnippet end
  }
  duration = omp_get_wtime()-tstart;
  sum = matsum( matrix,matrix_size );
  printf("Wrong way  time=%e, sum=%e\n",duration,sum);
  //  noopt( matrix,matrix_size );
  
  /*
   * The right way
   */
  matinit( matrix,matrix_size );
  tstart = omp_get_wtime();
  for ( nex=0; nex<100; ++nex) {
    #pragma omp parallel for
    for ( int i=0; i<matrix_size; ++i )
      for ( int j=0; j<matrix_size; ++j )
        matrix[INDEX(i,j)] *= 3.14;
  }
  duration = omp_get_wtime()-tstart;
  sum = matsum( matrix,matrix_size );
  printf("Right way  time=%e, sum=%e\n",duration,sum);
  noopt( matrix,matrix_size );
  
  
  return 0;
}
