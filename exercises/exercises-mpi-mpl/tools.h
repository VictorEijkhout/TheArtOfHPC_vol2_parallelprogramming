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
#include <mpl/mpl.hpp>
#include <math.h>

bool isapprox(int x,int y);
bool isapprox(double x,double y);
bool isapprox(float x,float y);

double array_error(const std::vector<double> &ref_array,const std::vector<double> &value_array);
int test_all_the_same_int( int value,const mpl::communicator& comm );
void print_final_result( bool cond,const mpl::communicator& comm );
void error_process_print(int error_proc, const mpl::communicator& comm);
