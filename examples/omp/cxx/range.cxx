/****************************************************************
 ****
 **** This file belongs with the course
 **** Introduction to Scientific Programming in C++/Fortran2003
 **** copyright 2016-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** range.cxx : C++20 ranges in OpenMP
 ****
 ****************************************************************/

#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <tuple>
using std::pair;

#include <algorithm>
#include <numeric>
using std::iota;

#include "omp.h"
#include <ranges>

int main()
{

  vector<long> data(10000000);
#pragma omp parallel for
  for ( int i=0; i<data.size(); i++ ) 
    data[i] = i+1;

  for ( auto [variant,msg] :
	  vector<pair<int,string>>{
	  { 1,string("sum of vector: ") },
	    { 2,string("sum w/ drop 1: ") },
	      { 3,string("sum times 2  : ") }
	  } ) {
    long count;
    double start,duration,msec;
    
    start = omp_get_wtime();
    count = 0;

    if (variant==1) {
      //codesnippet omprangereduct
#     pragma omp parallel for reduction(+:count)
      for ( auto e : data )
	count += e;
      //codesnippet end
    } else if (variant==2) {
      //codesnippet omprangelib
#     pragma omp parallel for reduction(+:count)
      for ( auto e : data
	      | std::ranges::views::drop(1) )
	count += e;
      //codesnippet end
    } else if (variant==3) {
      //codesnippet omprangelib
#     pragma omp parallel for reduction(+:count)
      for ( auto e : data
	      | std::ranges::views::transform
	      ( []( auto e ) { return 2*e; } ) )
	count += e;
      //codesnippet end
    } else throw("unknown variant");

    duration = omp_get_wtime()-start;
    msec = static_cast<int>( 1000000*duration )/1000.;
    cout << msg << count << " in " << msec << '\n';
  }

  return 0;
}
