/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2022-2023
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
template<typename T>
class NewVector {
protected:
  T *storage;
  int s;
//codesnippet end
//codesnippet classwithiterdata
public:
  NewVector( int s )
    : s(s){
    storage = new T[s]; };
  ~NewVector() { delete storage; };
  //codesnippet end
  T at(int i) const { if (i<0 or i>=s) throw;
    return storage[i]; };
  T& at(int i) { if (i<0 or i>=s) throw;
    return storage[i]; };

//codesnippet classwithiter
public:
  // iterator stuff
  class iter;
  iter begin();
  iter end();
//codesnippet end

//codesnippet classwithiter
};
//codesnippet end

//codesnippet classwithiteriter
template<typename T>
class NewVector<T>::iter {
private: T *searcher;
//codesnippet end
public:
  iter( T *searcher );
  //codesnippet omprandaccess
  NewVector<T>::iter& operator++();
  T& operator*(); 
  bool operator==( const NewVector::iter &other ) const;
  bool operator!=( const NewVector::iter &other ) const;
  // needed to OpenMP
  int operator-
      ( const NewVector::iter& other ) const;
  NewVector<T>::iter& operator+=( int add );
  //codesnippet end
//codesnippet classwithiteriter
};
//codesnippet end

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
  NewVector<float> v(s);
  //codesnippet end
  for ( int i=0; i<s; i++ )
    v.at(i) = i+.5;

  //codesnippet ompcustompar
  #pragma omp parallel for
  for ( auto e : v )
    cout << e << " ";
  //codesnippet end
  cout << '\n';

  return 0;
}

//codesnippet classwithiteriter
template<typename T>
NewVector<T>::iter::iter( T* searcher )
  : searcher(searcher) {};
template<typename T>
NewVector<T>::iter NewVector<T>::begin() {
  return NewVector<T>::iter(storage); };
template<typename T>
NewVector<T>::iter NewVector<T>::end()   {
  return NewVector<T>::iter(storage+NewVector<T>::s); };
//codesnippet end

//answersnippet classwithitersol1
template<typename T>
bool  NewVector<T>::iter::operator==
    ( const NewVector<T>::iter &other ) const {
  return searcher==other.searcher; };
template<typename T>
bool  NewVector<T>::iter::operator!=
    ( const NewVector<T>::iter &other ) const {
  return searcher!=other.searcher; };
template<typename T>
NewVector<T>::iter& NewVector<T>::iter::operator++() {
  searcher++; return *this; };
template<typename T>
NewVector<T>::iter& NewVector<T>::iter::operator+=( int add ) {
  searcher += add; return *this; };
//answersnippet end

//answersnippet classwithitersol2
template<typename T>
T&  NewVector<T>::iter::operator*() {
  return *searcher; };
// needed for OpenMP
template<typename T>
int   NewVector<T>::iter::operator-
    ( const NewVector<T>::iter& other ) const {
  return searcher-other.searcher; };
//answersnippet end
