/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% charcount.cxx : counting letters by reduction on std::map
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cout;
#include <string>
using std::string;

#include <map>
using std::map;

//codesnippet ompbincounter
template<typename key>
class bincounter : public map<key,int> {
public:
  // merge this with other map
  void operator+=
      ( const bincounter<key>& other ) {
    for ( auto [k,v] : other )
      if ( map<key,int>::contains(k) )
	this->at(k) += v;
      else
	this->insert( {k,v} );
  };
  // insert one char in this map
  void inc(char k) {
    if ( map<key,int>::contains(k) )
      this->at(k) += 1;
    else
      this->insert( {k,1} );
  };
};
//codesnippet end

#include <omp.h>

int main() {

  const int nreps = 100000;
  string foxtext{"the quick brown fox jumps over the lazy dog"};
  string text;
  for (int irep=0; irep<nreps; irep++)
    text += foxtext;

  auto tstart = omp_get_wtime();
  //codesnippet ompbinreduce
  /*
   * Reduction loop in main program
   */
  bincounter<char> charcount;
#pragma omp parallel for reduction(+ : charcount)
  for ( int i=0; i<text.size(); i++ )
    charcount.inc( text[i] );
  //codesnippet end
  auto duration = omp_get_wtime()-tstart;

  cout << "Resulting count\n";
  for ( auto& [c,n] : charcount )
    cout << "'" << c << "' : " << n << ", ";
  cout << " in " << .001 * static_cast<int>(duration*1000000) << " msec"
       << "\n";
  
  return 0;
}
