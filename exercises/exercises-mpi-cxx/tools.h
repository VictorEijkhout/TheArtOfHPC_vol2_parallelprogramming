/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2021
 ****
 **** tools.h : support routine prototypes
 ****
 ****************************************************************/

#include <vector>
#include <math.h>
#include "mpi.h"

#define ISAPPROX(x,y) \
  ( ( x==0. && fabs(y)<1.e-14 ) || ( y==0. && fabs(x)<1.e-14 ) || \
    fabs(x-y)/fabs(x)<1.e-14 )

double array_error(std::vector<double> ref_array,std::vector<double> value_array);
int test_all_the_same_int( int value,MPI_Comm comm );
void print_final_result( int cond,MPI_Comm comm );

