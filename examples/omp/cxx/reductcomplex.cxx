/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2023
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

  vector<complex<double>> vec(500, complex<double>(.1,.1) );
  auto red = complex_func(vec);
  cout << red << "\n";

  return 0;
}
