/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% heat.cxx : simple heat equation
   %%%%    naive allocation strategy
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <vector>
#include <cstdio>

#include <cassert>
#include <execution>
using namespace std;

#include <omp.h>

#ifndef N
#define N 10000000
#endif

int main(int argc,char **argv) {

  for ( auto omp : {false,true} ) {

    /*
     * Create a vector initialized to zero;
     * but set right boundary to one
     */
    vector<double> x(N),y(N);
    x.back() = 1.; y.back() = 1.;

    /*
     * Do a timing loop of 1000 iterations heat equation
     * (actually, plain averaging)
     */
    double timer = omp_get_wtime();
    for (int it=0; it<1000; it++) {

      if (omp) {
#pragma omp parallel for
	for (int i=1; i<y.size()-1; i++)
	  y[i] = ( x[i-1]+x[i]+x[i+1] )/3.;
#pragma omp parallel for
	for (int i=1; i<x.size()-1; i++)
	  x[i] = y[i];

      } else {

	int i=1;
	for_each( execution::par,
		  y.begin()+1,y.end()-1,
		  [x,&i] ( double& y ) {
		    y = ( x[i-1]+x[i]+x[i+1] )/3.;
		    i++;
		  }
		  );
	assert( i==y.size()-1 );
	i = 0;
	for_each( execution::par,
		  x.begin(),x.end(),
		  [y,&i] ( double& x ) {
		    x = y[i];
		    i++;
		  }
		  );
      }
    }
    timer = omp_get_wtime()-timer;
    double s=0;
    for (int i=0; i<N; i++)
      s += y[i];

#pragma omp parallel
#pragma omp single
    printf("Time=%8.4f #ops=%9.3e on %d threads (checksum %e)\n",
	   timer,(double)(2*100*N),omp_get_num_threads(),s);
  }

  return 0;
}
