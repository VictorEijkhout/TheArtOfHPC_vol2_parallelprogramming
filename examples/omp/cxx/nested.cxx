/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% .cxx : C++ OpenMP example for
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;

#include <omp.h>

//codesnippet nestclass
class withnest {
public:
  void f() {
    stringstream ss;
    ss 
      << omp_get_num_threads()
      << '\n';
    cout << ss.str();
  };
};
int main() {
  withnest my_object;
#pragma omp parallel
  my_object.f();
//codesnippet end
  
  return 0;
}
