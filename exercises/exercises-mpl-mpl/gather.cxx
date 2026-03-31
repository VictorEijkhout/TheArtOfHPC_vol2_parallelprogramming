/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2020-2026
 ****
 **** MPI Exercise for the use of gather
 ****
 ****************************************************************/

#include <complex>
#include <iostream>
#include <format>
#include <random>
#include <vector>
#include <mpl/mpl.hpp>

float random_float() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(0.0, 1.0);
  return dis(gen);
}

template<>
class std::formatter<std::complex<float>>{
public:
  constexpr auto parse( std::format_parse_context& ctx ) {
    return ctx.begin(); };
  auto format( const std::complex<float>& z,std::format_context& ctx ) const {
    return std::format_to
      ( ctx.out(),"({},{})",z.real(),z.imag());
  };
};

template<>
class std::formatter<std::vector<std::complex<float>>>{
public:
  constexpr auto parse( std::format_parse_context& ctx ) {
    return ctx.begin(); };
  auto format( const std::vector<std::complex<float>>& z,std::format_context& ctx ) const {
    return std::format_to
      ( ctx.out(),"({},{})",z[0].real(),z[0].imag());
  };
};

int main() {

  const mpl::communicator &comm_world = mpl::environment::comm_world();
  
  int nprocs, procno;
  nprocs = comm_world.size();
  procno = comm_world.rank();
  //solved

  //
  // Every process goes its own random number
  //
  std::complex<float> myrandom = {random_float(),random_float()};
  std::complex<float> maxrandom;

  //  std::cout << myrandom << '\n';

  comm_world.allreduce
    ( [] (std::complex<float> x,std::complex<float> y) {
          return std::norm(x)>std::norm(y) ? x : y ; },
    myrandom,maxrandom );

  // Exercise:
  // gather all the random numbers on a root process
  int root = nprocs-1;
  if (procno==root) {
    std::vector<std::complex<float>> gathered(nprocs);
/**** your code here ****/
    std::cout << std::format( "gathered: {}",gathered ) << '\n';
  } else {
    comm_world.gather( root, myrandom );
  }

  return 0;
}
