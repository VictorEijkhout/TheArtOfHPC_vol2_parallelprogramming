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

#include <omp.h>

//codesnippet nestclass
class c {
public:
  void f() {
    cout << omp_get_num_threads() << '\n';
  };
};
int main() {
  c my_object;
#pragma omp parallel
  my_object.f();
//codesnippet end
  
  return 0;
}
