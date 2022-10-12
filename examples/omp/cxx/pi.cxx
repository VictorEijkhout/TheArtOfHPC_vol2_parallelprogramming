/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2020
   %%%%
   %%%% pi.cxx : compute pi
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

#include <cmath>
#include <omp.h>

using floattype = float;

int main(int argc,char **argv) {

  int N=10000;
  if (argc>1) N = atoi(argv[1]);
  floattype h = static_cast<floattype>(1)/static_cast<floattype>(N);

  floattype pi4 = 0.; 
  double tstart = omp_get_wtime();
  //for (int isample=N-1; isample>=0; isample--) {
  // #pragma omp parallel for reduction(+:pi4)
  for (int isample=0; isample<N; isample++) {
    floattype xsample = (N-1-isample) * h;
    floattype y2 = 1-xsample*xsample;
    if (y2<0) throw(1);
    floattype y = sqrt(y2);
    pi4 += y;
    if (isample%(N/20)==0) cout << y2 << "," << y << ":" << pi4 << endl;
  }
  pi4 *= h;
  double duration = omp_get_wtime()-tstart;

  stringstream proctext;
  proctext << "Pi computed with " << N << " samples: " << 4*pi4 << "; time=" << duration << endl;
  cout << proctext.str();

  return 0;
}
