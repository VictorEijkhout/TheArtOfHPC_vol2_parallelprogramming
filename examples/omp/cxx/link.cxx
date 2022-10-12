/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% link.cxx : simple hello world
   %%%%
   %%%% THIS EXAMPLE IS POINTLESS
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <sstream>
using std::stringstream;

#include "omp.h"

int main(int argc,char **argv) {

  int p{0};
#pragma omp parallel shared(p)
  {
    int n;
    while (p<1000) {
      stringstream proctext;
#pragma omp critical
      p++;
      n = omp_get_thread_num();
      proctext << n << " " << p << endl;
      cout << proctext.str();
    }
  }

  return 0;
}
