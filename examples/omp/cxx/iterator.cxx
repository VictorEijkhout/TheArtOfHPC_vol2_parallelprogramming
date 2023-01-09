/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2022
   %%%%
   %%%% iterator.cxx : iterating on custom iterator
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include "omp.h"

//codesnippet classwithiter
class NewVector {
//codesnippet end
protected:
  int *storage;
  int s;
public:
  NewVector( int s )
    : s(s){
    storage = new int[s]; };
  ~NewVector() { delete storage; };
  int at(int i) const { if (i<0 or i>=s) throw;
    return storage[i]; };
  int& at(int i) { if (i<0 or i>=s) throw;
    return storage[i]; };

//codesnippet classwithiter
public:
  // iterator stuff
  class iter;
  iter begin();
  iter end();

};
//codesnippet end

class NewVector::iter {
private: int *searcher;
public:
  iter( int *searcher );
  //codesnippet omprandaccess
  NewVector::iter& operator++();
  int& operator*(); 
  bool operator==( const NewVector::iter &other ) const;
  bool operator!=( const NewVector::iter &other ) const;
  // needed to OpenMP
  int operator-( const NewVector::iter& other ) const;
  NewVector::iter& operator+=( int add );
  //codesnippet end
};

int main() {

  //codesnippet ompinlambda
  const int s = [] () {
    int s;
#   pragma omp parallel
#   pragma omp master
    s = 2 * omp_get_num_threads();
    return s; }();
  //codesnippet end

  //codesnippet ompcustompar
  NewVector v(s);
  //codesnippet end
  for ( int i=0; i<s; i++ )
    v.at(i) = i+1;

  //codesnippet ompcustompar
  #pragma omp parallel for
  for ( auto e : v )
    cout << e << " ";
  //codesnippet end
  cout << '\n';

  return 0;
}

NewVector::iter::iter( int *searcher ) : searcher(searcher) {};
NewVector::iter NewVector::begin() { return NewVector::iter(storage); };
NewVector::iter NewVector::end()   { return NewVector::iter(storage+NewVector::s); };

NewVector::iter& NewVector::iter::operator++() { searcher++; return *this; };
int&  NewVector::iter::operator*() { return *searcher; };
bool  NewVector::iter::operator==( const NewVector::iter &other ) const {
  return searcher==other.searcher; };
bool  NewVector::iter::operator!=( const NewVector::iter &other ) const {
  return searcher!=other.searcher; };
// needed to OpenMP
int   NewVector::iter::operator-( const NewVector::iter& other ) const {
  return searcher-other.searcher; };
NewVector::iter& NewVector::iter::operator+=( int add ) { searcher += add; return *this; };
