/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%%   "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2019-2020
   %%%%
   %%%% onenorm.cxx : user-defined one-norm using MPL
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <sstream>

#include <functional>
#include <random>
#include <cmath>
using namespace std;
#include <mpl/mpl.hpp>

template<typename T>
class OneNorm : std::function<T(T,T)> {
private:
  void one_norm(void *in,void *inout,int *len,MPI_Datatype *type) const {
    // r is the already reduced value, n is the new value
    T n = *(T*)in, r = *(T*)inout;
    T m;
    
    /*
     * Exercise:
     * -- write the correct reduction step 
     *    for computing a 1-norm
     */
/**** your code here ****/
#ifdef DEBUG
    stringstream proctext;
    proctext << "combine " << r << " " << n << ": " << m << "\n";
    cerr << proctext.str();
#endif
    *(T*)inout = m;  
  };
public:
  T operator()(T in,T inout) const {
    T input = in, update = inout;
    int len = 1;
    MPI_Datatype type ; // MPI_FLOAT;
    MPI_Type_match_size(MPI_TYPECLASS_REAL,sizeof(in),&type);
    one_norm( &input,&update,&len,&type );
    return update;
  };
};

int main(int argc,char **argv) {

  /*
   * Set up communicator, rank size
   */
  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  nprocs = comm_world.size();
  procno = comm_world.rank();

  // set the default random number generator
  std::random_device r;
  std::default_random_engine generator{ r() };
  // distribution: real between 0 and 1
  std::uniform_real_distribution<float> distribution(0.,1.);

  /* 
   * Data specific for this program:
   * every process stores one number, minus-one,
   * which makes the one-norm equal to the 
   * number of processes.
   */  
  float data,data_one_norm;
  data = distribution(generator);

  /*
   * Create the operator,
   * do the reduction,
   * free the operator.
   */
  comm_world.allreduce( OneNorm<float>(),data,data_one_norm );

  /*
   * Correctness test:
   * gather and sum, compare to reduced result.
   */
  {
    float actual_norm{0.};
    vector<float> alldata(nprocs);
    comm_world.gather(0,data,alldata.data());
    for ( auto x : alldata ) actual_norm += abs(x);
    if (procno==0) {
      stringstream proctext;
      float relative_error = abs( (data_one_norm-actual_norm)/actual_norm );
      const float tolerance = 1.e-4;
      if (relative_error>tolerance) {
	proctext << "Result " << data_one_norm
                 << " should be " << actual_norm << "\n";
      } else if (procno==0) {
        proctext << "User-defined reduction: " << data_one_norm
                 << " correct with relative error " << relative_error << "\n";
      }
      cerr << proctext.str();
    }
  }

  return 0;
}
