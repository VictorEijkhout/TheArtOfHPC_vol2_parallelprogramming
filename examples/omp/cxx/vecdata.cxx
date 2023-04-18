/****************************************************************
 ****
 **** This file belongs with the course
 **** Introduction to Scientific Programming in C++/Fortran2003
 **** copyright 2016-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** vecdata.cxx : range based loops and other vector operations
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

#include <omp.h>

int main() {


  {
  vector<float> values(100);
#pragma omp parallel for 
    for ( int irow=0; irow<values.size(); irow++ ) {
      auto elt = values.data()+irow;
      *elt = 5.f;
    }
    float sum{0.f};
#pragma omp parallel for reduction(+:sum)
    for ( auto elt : values ) {
      sum += elt;
    }
    cout << "sum of fives: " << sum << "\n";
  }

  {
    //codesnippet omprangeloop
    vector<float> values(100);

#pragma omp parallel for 
    for ( auto& elt : values ) {
      elt = 5.f;
    }

    float sum{0.f};
#pragma omp parallel for reduction(+:sum)
    for ( auto elt : values ) {
      sum += elt;
    }
    //codesnippet end
    cout << "sum of fives: " << sum << "\n";
  }

  return 0;
}

