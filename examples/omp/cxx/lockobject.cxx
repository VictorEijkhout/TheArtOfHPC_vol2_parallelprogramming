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
using std::cout;
#include <vector>
using std::vector;

#include <cmath>

#include <thread>
#include <atomic>

#include <omp.h>

#ifdef NOLOCK
#define omp_set_lock(x)
#define omp_unset_lock(x)
#endif

//codesnippet lockobject
class atomic_int {
private:
  omp_lock_t the_lock;
  int _value{0};
  bool delay;
public:
  atomic_int(bool delay=false)
    : delay(delay) {
    omp_init_lock(&the_lock);
  };
  atomic_int( const atomic_int& )
      = delete;
  atomic_int& operator=( const atomic_int& )
      = delete;
  ~atomic_int() {
    omp_destroy_lock(&the_lock);
  };
  int operator +=( int i ) {
    //codesnippet end
    float s = i;
    if (delay) {
      // let's waste a little time,
      // otherwise the threads finish before they start
      for (int i=0; i<1000; i++)
        s += sin(i)*sin(i);
    }
    //codesnippet lockobject
    // atomic increment
    omp_set_lock(&the_lock);
    _value += i; int rv = _value;
    omp_unset_lock(&the_lock);
    return rv;
  };
  auto value() { return _value; };
};
//codesnippet end

#define NTHREADS 50

int main() {

  for ( auto run : {1,2,3} ) {
    bool delay = (run==1);
    int nops = [delay] () -> int {
      if (delay) return 100 ; else return 1000 ; }();
    
    int result;
    auto tstart = omp_get_wtime();
    if (run<3) {
      /*
       * Create a bunch of threads, that
       * each do a bunch of updates
       */
      atomic_int my_object(delay);
      vector<std::thread> threads;
      //codesnippet lockobjectuse
      for (int ithread=0; ithread<NTHREADS; ithread++) {
        threads.push_back
          ( std::thread(
                   [=,&my_object] () {
                     for (int iop=0; iop<nops; iop++)
                       my_object += 1; } ) );
      }
      for ( auto &t : threads )
        t.join();
      //codesnippet end
      result = my_object.value();

    } else {
      //codesnippet atomicintuse
      std::atomic<int> my_object{0};
      #pragma omp parallel for
      for ( size_t update=0; update<NTHREADS*nops; update++) {
        my_object += 1;
      }
      result = my_object;
      //codesnippet end
    }
    auto duration = omp_get_wtime()-tstart;
    /* 
     * Check that no updates have gone lost
     */
    cout << "Did " << NTHREADS * nops << " updates, over " << NTHREADS
         << " threads, resulting in " << result << '\n'
         << " runtime=" << duration << '\n';
  }
  
  return 0;
}
