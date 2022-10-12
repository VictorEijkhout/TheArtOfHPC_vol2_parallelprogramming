/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2020-2022
   %%%%
   %%%% reduceuser.cxx : user-defined reduction in MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <functional>
#include <vector>
#include <mpl/mpl.hpp>
 
// calculate least common multiple of two arguments 
template<typename T>
class lcm {
public:
  T operator()(T a, T b) {
    T zero=T();
    T t((a/gcd(a, b))*b);
    if (t<zero)
      return -t;
    return t;
  }
private:
  // helper: calculate greatest common divisor
  T gcd(T a, T b) {
    T zero=T(), t;
    if (a<zero) a=-a;
    if (b<zero) b=-b;
    while (b>zero) {
      t=a%b;  a=b;  b=t;
    }
    return a;
  }
};
 
int main() {
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  // generate data
  std::srand(std::time(0)*comm_world.rank());  // random seed
  int v=std::rand()%12+1;
  // calculate least common multiple and send result to rank 0
  if (comm_world.rank()==0) {
    int result;
    // calculate least common multiple
    comm_world.reduce(lcm<int>(), 0, v, result);
    // display data from all ranks
    std::cout << "Arguments:\n";
    for (int r=0; r<comm_world.size(); ++r) {
      if (r>0)
        comm_world.recv(v, r);
      std::cout << v << '\n';
    }
    // display results of global reduction
    std::cout << "\nResult:\n";
    std::cout << result << '\n';
  } else {
    // calculate least common multiple
    comm_world.reduce(lcm<int>(), 0, v);
    // send data to rank 0 for display
    comm_world.send(v, 0);
  }
  {
    int data = comm_world.rank();
    comm_world.reduce
      ( [] (int i,int j) -> int 
           { return i+j; },
	0,data );
    if (comm_world.rank()==0)
      std::cout << " reduce: " << data << std::endl;
  }
  
  return EXIT_SUCCESS;
}

