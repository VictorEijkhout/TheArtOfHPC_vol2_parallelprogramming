/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-8
   %%%%
   %%%% ureduct.c : user-defined reduction
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <algorithm>
#include <iostream>
using std::cout;
#include <vector>
using std::vector;

#include <omp.h>

template<typename T>
T reduce_without_zero(T r,T n) {
  // r is the already reduced value, n is the new value
  T m;
  if (r<0){ // initializer
    if (n!=0)
      m = n;
    else
      m = r;
  } else if (n<0) { // initializer
    if (r!=0)
      m = r;
    else
      m = n;
  } else if (n==0) { // new value is zero: keep r
    m = r;
  } else if (n<r) { // new value is less but not zero: use n
    m = n;
  } else { // new value is more: use r
    m = r;
  };
  return m;
}

template<typename T>
T generic_reduction( vector<T> tdata ) {
#pragma omp declare reduction					\
  (rwzt:T:omp_out=reduce_without_zero<T>(omp_out,omp_in))	\
  initializer(omp_priv=-1.f)

  T tmin = -1;
#pragma omp parallel for reduction(rwzt:tmin)
  for (int id=0; id<tdata.size(); id++)
    tmin = reduce_without_zero<T>(tmin,tdata[id]);
  return tmin;
};

int main(int argc,char **argv) {

  int ndata = 10;
  vector<int> idata = {2,3,0,5,0,1,8,12,4,0};
  vector<float> fdata = {2.5f,3.5f,0.0f,5.5f,0.0f,1.5f,8.5f,12.5f,4.5f,0.0f};

  /*
   * Int case
   */
  cout << "\nInt case\n";

  int ireduct=2000000;
  for_each( idata.begin(),idata.end(),
	    [&ireduct] (int &r) { if (r<ireduct && r>0) ireduct = r; }
	    );
#pragma omp declare reduction \
  (rwzi:int:omp_out=reduce_without_zero<int>(omp_out,omp_in)) \
  initializer(omp_priv=-1)

  {
    int imin = -2;
    for (int id=0; id<idata.size(); id++)
      imin = reduce_without_zero<int>(imin,idata[id]);
    if (imin!=ireduct)
      cout << "Sequential: wrong reduced value: " << imin << ", s/b " << ireduct << "\n";
    else
      cout << "Sequential case succeeded\n";
  }

  {
    int imin = -1;
#pragma omp parallel for reduction(rwzi:imin)
    for (int id=0; id<idata.size(); id++)
      imin = reduce_without_zero<int>(imin,idata[id]);

    if (imin!=ireduct)
      cout << "Parallel: wrong reduced value: " << imin << ", s/b " << ireduct << "\n";
    else
      cout << "Parallel case reduction correct\n";
  }

  /*
   * Float case
   */
  cout << "\nFloat case\n";

  float freduct=2000000;
  for_each( fdata.begin(),fdata.end(),
	    [&freduct] (float &r) { if (r<freduct && r>0) freduct = r; }
	    );

#pragma omp declare reduction \
  (rwzf:float:omp_out=reduce_without_zero<float>(omp_out,omp_in)) \
  initializer(omp_priv=-1.f)

  {
    float fmin = -2;
    for (int id=0; id<fdata.size(); id++)
      fmin = reduce_without_zero<float>(fmin,fdata[id]);
    if (fmin!=freduct)
      cout << "Sequential: wrong reduced value: " << fmin << ", s/b " << freduct << "\n";
    else
      cout << "Sequential case succeeded\n";
  }
  
  {
    float fmin = -1;
#pragma omp parallel for reduction(rwzf:fmin)
    for (int id=0; id<fdata.size(); id++)
      fmin = reduce_without_zero<float>(fmin,fdata[id]);

    if (fmin!=freduct)
      cout << "Parallel: wrong reduced value: " << fmin << ", s/b " << freduct << "\n";
    else
      cout << "Parallel case reduction correct\n";
  }
  
  /*
   * Generic case
   */
  cout << "\nGeneric case\n";

  auto tmin = generic_reduction<float>(fdata);
  if (tmin!=freduct)
    cout << "Parallel: wrong reduced value: " << tmin << ", s/b " << freduct << "\n";
  else
    cout << "Parallel: case reduction correct\n";

  return 0;
}
