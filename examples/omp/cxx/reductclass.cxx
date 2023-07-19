/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% reductclass : reduction on operator
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

//codesnippet ompclassop
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
//codesnippet end

int main() {

//codesnippet ompreductop
  vector< Thing > things(500,Thing(1.f) );
  Thing result(0.f);
#pragma omp parallel for reduction( +:result )
  for ( const auto& t : things )
    result = result + t;
//codesnippet end
  
  return 0;
}
