/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% reductcomplex : complex reduction is not built in
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

int main() {

  vector<string> letters(26);
  for ( int a=0; a<26; a++ ) {
    string l{""};
    l += static_cast<char>( a+'a' );
    letters[a] = l;
    //cout << l;
  }
  //  cout << '\n';

//codesnippet ompreductstr
  string result{""};
#pragma omp parallel for reduction( +:result )
  for ( const auto& t : letters )
    result = result + t;
//codesnippet end
  cout << "result: " << result << '\n';

  return 0;
}
