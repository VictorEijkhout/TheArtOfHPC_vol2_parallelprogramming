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

  {
    vector<long> data(10000000);
    iota(data.begin(),data.end(),1L);
    long count;
    double start,duration,msec;
    
    start = omp_get_wtime();
    count = 0;
    //codesnippet omprangelib
#   pragma omp parallel for reduction(+:count)
    for ( auto e : data )
      count += e;
    //codesnippet end
    duration = omp_get_wtime()-start;
    msec = static_cast<int>( 1000000*duration )/1000.;
    cout << "sum of vector: " << count << " in " << msec << '\n';

    start = omp_get_wtime();
    count = 0;
    //codesnippet omprangelib
#   pragma omp parallel for reduction(+:count)
    for ( auto e : data
            | std::ranges::views::drop(1) )
      count += e;
    //codesnippet end
    duration = omp_get_wtime()-start;
    msec = static_cast<int>( 1000000*duration )/1000.;
    cout << "sum w/ drop 1: " << count << " in " << msec << '\n';

    start = omp_get_wtime();
    count = 0;
    //codesnippet omprangelib
#   pragma omp parallel for reduction(+:count)
    for ( auto e : data
            | std::ranges::views::transform
                ( []( auto e ) { return 2*e; } ) )
      count += e;
    //codesnippet end
    duration = omp_get_wtime()-start;
    msec = static_cast<int>( 1000000*duration )/1000.;
    cout << "sum times 2  : " << count << " in " << msec << '\n';

  }

  return 0;
}
