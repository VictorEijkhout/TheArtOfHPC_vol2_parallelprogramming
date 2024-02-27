/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2024
   %%%%
   %%%% directive.cxx : new directive syntax
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
using namespace std;

#include "omp.h"

int main(int argc,char **argv) {

  {
    //codesnippet ompcppdirective
    int nthreads;
    [[omp::directive( parallel ) ]]
    [[omp::directive( master ) ]]
    nthreads = omp_get_num_threads();
    //codesnippet end
    printf("Found %d threads\n",nthreads);

    //codesnippet privatevector
    vector<int> array(nthreads);
    #pragma omp parallel firstprivate(array)
    {
      int t = omp_get_thread_num();
      array[t] = t+1;
    }
    // ... print the array
    //codesnippet end

    printf("Private\n");
    printf("Array result:\n");
    for (int i=0; i<nthreads; i++)
      printf("%d:%d, ",i,array[i]);
    printf("\n");
    printf(".. private\n");
  }

  return 0;
}
