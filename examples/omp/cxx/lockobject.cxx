/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-2022
   %%%%
   %%%% lockobject.cxx : objects with a safe update method
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <cmath>

#include <omp.h>

#ifdef NOLOCK
#define omp_set_lock(x)
#define omp_unset_lock(x)
#endif

class object {
private:
  omp_lock_t the_lock;
  int _value{0};
public:
  object() {
    omp_init_lock(&the_lock);
  };
  ~object() {
    omp_destroy_lock(&the_lock);
  };
  int operator +=( int i ) {
    // let's waste a little time,
    // otherwise the threads finish before they start
    float s = i;
    for (int i=0; i<1000; i++)
      s += sin(i)*sin(i);

    // atomic increment
    omp_set_lock(&the_lock);
    _value += (s>0); int rv = _value;
    omp_unset_lock(&the_lock);
    return rv;
  };
  auto value() { return _value; };
};

#define NTHREADS 50
#define NOPS 100

int main() {

  /*
   * Create a bunch of threads, that
   * each do a bunch of updates
   */
  object my_object;
  vector<thread> threads;
  for (int ithread=0; ithread<NTHREADS; ithread++) {
    threads.push_back
      ( thread(
	       [&my_object] () {
		 for (int iop=0; iop<NOPS; iop++)
		   my_object += iop; } ) );
  }
  for ( auto &t : threads )
    t.join();

  /* 
   * Check that no updates have gone lost
   */
  cout << "Did " << NTHREADS * NOPS << " updates, over " << threads.size()
       << " threads, resulting in " << my_object.value() << endl;
  
  return 0;
}
