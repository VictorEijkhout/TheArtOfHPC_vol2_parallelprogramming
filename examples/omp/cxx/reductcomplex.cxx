/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% reductcomplex : complex reduction is not built in
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
#include <vector>
#include <cmath>
#include <algorithm>
#include <complex>
using namespace std;

class Thing {
private:
  float x;
public:
  Thing() : Thing( 0.f ) {};
  Thing( float x ) : x(x) {};
  Thing operator+( const Thing& other ) {
    return Thing( x + other.x );
  };
};

complex<double> complex_func(vector<complex<double>> x)
{
  long n = x.size();
  complex<double> res = 0.0;

  #pragma omp parallel for reduction(+:res)
  for (long i = 0; i < n; i++) {
    res += exp(pow(log(x[i]), 2));
  }
  
  return res;
}

int main() {

  vector<complex<double>> vec(500);
  auto red = complex_func(vec);
  cout << red << "\n";

  vector< Thing > things(500,Thing(1.f) );
  Thing result(0.f);
#pragma omp parallel for reduction( +:result )
  for ( const auto& t : things )
    result = result + t;
  
  return 0;
}
