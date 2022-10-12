/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Programming for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-2021
   %%%%
   %%%% treesearch.cxx : tasking example
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <random>
#include <vector>
#include <memory>
using namespace std;

#include <omp.h>

using floattype = double;

class Node {
private:
  floattype value{0};
  shared_ptr<Node> left{nullptr},right{nullptr};
public:
  Node( floattype v ) : value(v) {};
  void insert( floattype v ) {
    if (v<value) {
      if (left==nullptr) left = make_shared<Node>(v);
      else left->insert(v);
    } else if (v > value ) {
      if (right==nullptr) right = make_shared<Node>(v);
      else right->insert(v);
    }
  };
  int nnodes() {
    int n{1};
    if (left!=nullptr)
#pragma omp task shared(n)
      n += left->nnodes();
    if (right!=nullptr)
#pragma omp task shared(n)
      n += right->nnodes();
    return n;
  };
  int depth() {
    int nleft{0},nright{0};
    if (left!=nullptr)
      nleft = left->depth();
    if (right!=nullptr)
      nright = right->depth();
    return 1 + max(nleft,nright);
  };
  floattype sum() {
    floattype leftsum{0.}, rightsum{0.};
    if (left!=nullptr)
#pragma omp task shared(leftsum)
      leftsum = left->sum();
    if (right!=nullptr)
#pragma omp task shared(rightsum)
      rightsum = right->sum();
#pragma omp taskwait
    return value + leftsum + rightsum;
  };
};

int main(int argc, char **argv) {

  std::random_device r;
  std::default_random_engine generator{r()};
  std::uniform_real_distribution<floattype> distribution(0.,1.);
  
  floattype checksum = distribution(generator);
  Node root( checksum );
  int nnodes = 1000000;
  for (int i=0; i<nnodes; i++) {
    auto v = distribution(generator);
    checksum += v;
    root.insert( v );
  }
  floattype sum{0.};
  double tstart = omp_get_wtime();
#pragma omp parallel
#pragma omp single
  sum = root.sum();
  double duration = omp_get_wtime()-tstart;
  
  cout << "node count " << root.nnodes() << ", check: " << nnodes+1
       << "; depth=" << root.depth() << endl;
  cout << "sum: " << sum << ", checksum: " << checksum << endl;
  cout << ".. time for computation: " << duration << endl;

  return 0;
}
