/****************************************************************
 ****
 **** This file belongs with the course
 **** Introduction to Scientific Programming in C++/Fortran2003
 **** copyright 2016-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** iota.cxx : C++20 ranges iota_view
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

int f(int i) { return i+1; };

int main()
{

  const int N = 10000000;
  //codesnippet ompiotaview
  vector<long> data(N);
# pragma omp parallel for
  for ( auto i : std::ranges::iota_view( static_cast<size_t>(0),data.size() ) )
    data[i] = f(i);
  //codesnippet end

  return 0;
}
