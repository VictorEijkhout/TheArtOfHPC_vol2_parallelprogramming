/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2023
   %%%%
   %%%% filewrite.c : MPI IO example
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  /*
   * Set up world communicator
   * and compute rank/size
   */
  const mpl::communicator &comm_world = mpl::environment::comm_world();
  int nprocs,procno;
  nprocs = comm_world.size();
  procno = comm_world.rank();

  //codesnippet mplfilebasic
  mpl::file mpifile
    (comm_world,"filewrite.dat",
     mpl::file::access_mode::create | mpl::file::access_mode::write_only
     );
  //codesnippet end

  //codesnippet mplfilewrite
  mpifile.write_at
    ( /* offset: */ procno*sizeof(int),
      /* data: */ procno );
  //codesnippet end

  //codesnippet mplfilebasic
  mpifile.close();
  //codesnippet end

  // if (procno==0) {
  //   int ncorrect=0;
  //   FILE *f;
  //   f = fopen("filewrite.dat","r");
  //   for (int i=0; i<nprocs; i++) {
  //     int fromfile,success;
  //     success = fread(&fromfile,sizeof(int),1,f);
  //     if (success==EOF) {
  //       printf("Premature end of file\n"); break;
  //     }
  //     if (fromfile==i)
  // 	ncorrect++;
  //     else
  //       printf("Error %d:%d\n",i,fromfile);
  //   }
  //   fclose(f);
  //   printf("Finished: all %d correct\n",ncorrect);
  // }

  return 0;
}
