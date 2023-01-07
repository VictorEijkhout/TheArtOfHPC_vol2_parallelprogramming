/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% reductarray.c : reduction on array
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;
#include <omp.h>

int main(int argc,char **argv) {

  int nthreads;
#pragma omp parallel
#pragma omp single
  nthreads = omp_get_num_threads();

  {
    int data[nthreads];
    memset(data,0,nthreads*sizeof(int));
#pragma omp parallel for schedule(static,1) \
      reduction(+:data[:nthreads])
    for (int it=0; it<nthreads; it++) {
      for (int i=0; i<nthreads; i++)
	data[i]++;
    }

    printf("Data should be ===%d:",nthreads);
    for (int i=0; i<nthreads; i++)
      printf(" %d",data[i]);
    printf("\n");
  }

  {
    int *data = (int*) malloc(nthreads*sizeof(int));
    memset(data,0,nthreads*sizeof(int));
#pragma omp parallel for schedule(static,1) \
      reduction(+:data[:nthreads])
    for (int it=0; it<nthreads; it++) {
      for (int i=0; i<nthreads; i++)
	data[i]++;
    }

    printf("Data should be ===%d:",nthreads);
    for (int i=0; i<nthreads; i++)
      printf(" %d",data[i]);
    printf("\n");
    free(data);
  }

  { // C++ vectors
    //codesnippet cxxreductvector
    vector<int> data(nthreads,0);
    int *datadata = data.data();
#pragma omp parallel for schedule(static,1) \
      reduction(+:datadata[:nthreads])
    for (int it=0; it<nthreads; it++) {
      for (int i=0; i<nthreads; i++)
	datadata[i]++;
    }
    //codesnippet end
    printf("Data should be ===%d:",nthreads);
    for (int i=0; i<nthreads; i++)
      printf(" %d",data[i]);
    printf("\n");
  }

  return 0;
}
