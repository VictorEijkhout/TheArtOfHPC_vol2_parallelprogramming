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
#include <vector>
using std::vector;

#include <cmath>
#include <omp.h>

int main(int argc,char **argv) {

  int N=10000;
  if (argc>1) N = atoi(argv[1]);
  float h = (1./N);

  /*
   * We calculate pi by 
   * 1. calculating the values
   */
  vector<float> yvalues(N);

  double tstart = omp_get_wtime();
#pragma omp parallel for
  for (int isample=0; isample<yvalues.size(); isample++) {
    float xsample = isample * h;
    float y = sqrt(1-xsample*xsample);
    yvalues[isample] = y;
  }

  /*
   * 2. adding up the samples
   */
  float pi4 = 0.;
  vector<float>::iterator ysample;
#pragma omp parallel for reduction(+:pi4) shared(yvalues) private(ysample)
  for (ysample=yvalues.begin(); ysample<yvalues.end(); ++ysample) {
    pi4 += *ysample * h;
  }
  double duration = omp_get_wtime()-tstart;

  stringstream proctext;
  proctext << "Pi computed with " << N << " samples: " << 4*pi4 << "; time=" << duration << endl;
  cout << proctext.str();

  return 0;
}
