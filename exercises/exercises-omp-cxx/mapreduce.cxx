/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2023
   %%%%
   %%%% mapreduce : mapreduce 
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <vector>

#include <chrono>
#include <map>
#include <random>

using namespace std;
using namespace std::chrono;

bool isprime( int n ) {
  return n==2 or
    [] (int n) -> bool {
      for ( int d=3; d<n/2; d+=2 )
	if (n%d==0) return false;
      return true;
    } (n);
};

class intcounter {
private:
  map<int,int> _counts;
public:
  intcounter() = default;
  void add(int n) {
    _counts[n]++;
  };
  intcounter operator+( const intcounter& other ) {
    /*
     * Code to merge two intcounter objects
     */
/**** your code here ****/
  };
  const map<int,int>& counts() const { return _counts; };
};


int main(int argc,char **argv) {

  /*
   * If this program is run with no arguments: run sequential & parallel
   * Otherwise:
   * 1 = sequential
   * 2 = parallel
   */
  auto variants = [argc,argv] () -> vector<int> {
    if (argc==1) return {1,2};
    else return {atoi(argv[1])}; }();

  /*
   * Set up a long vector of ints
   */
  vector<int> numbers(1'000'000'000);
  // random_device rd;
  std::default_random_engine random_generator;
  uniform_int_distribution<int> int_dist(0,99);
  for ( auto& n : numbers )
    n = int_dist(random_generator);

  /*
   * Just for debugging:
   * output the array
   */
  // cout << "numbers:";
  // for ( auto n : numbers )
  //   cout << " " << n;
  // cout << '\n';

  /*
   * Count primes two possible ways
   */
  for ( auto variant : variants ) {
    intcounter primecounter;
    using clock = system_clock;
    clock::time_point  before = clock::now();
    if ( variant==1 ) { // serial counting
      for ( auto n : numbers )
	if ( isprime(n) )
	  primecounter.add(n);
    } else if (variant==2) { // parallel counting
#     pragma omp parallel for reduction(+:primecounter)
      for ( auto n : numbers )
	if (isprime(n))
	  primecounter.add(n);
    }
    auto after = clock::now();
    cout << "Took: "
	 << duration_cast<milliseconds>(after-before).count()
	 << "ms\n";
    /*
     * Reporting
     */
    // cout << "Primes found:";
    // for ( auto [num,count] : primecounter.counts() )
    //   cout << " " << num << "x" << count;
    // cout << '\n';
  }

  return 0;
}
