#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#include "gather_sort_print.h"

void gather_sort_print
    ( int *my_counter_values,int n_my_counter_values, MPI_Comm comm ) {
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  int
    counter_values_counts[nprocs],
    counter_values_offsets[nprocs],
    *counter_values_gathered = NULL;
  MPI_Gather(&n_my_counter_values,1,MPI_INT,
	     counter_values_counts,1,MPI_INT,
	     0,comm);
  int total_n_counter_values = 0;
  if (procno==0) {
    for (int p=0; p<nprocs; p++) {
      counter_values_offsets[p] = total_n_counter_values;
      total_n_counter_values += counter_values_counts[p];
    }
    counter_values_gathered = (int*) malloc ( total_n_counter_values * sizeof(int) );
  }
  MPI_Gatherv( my_counter_values,n_my_counter_values,MPI_INT,
	       counter_values_gathered,counter_values_counts,counter_values_offsets,MPI_INT,
	       0,comm);
  if (procno==0) {
    // bubble sort. oh well
    for (int cstop=total_n_counter_values-2; cstop>=0; cstop--)
      for (int c=0; c<=cstop; c++)
	if (counter_values_gathered[c]>counter_values_gathered[c+1]) {
	  int t = counter_values_gathered[c+1];
	  counter_values_gathered[c+1] = counter_values_gathered[c];
	  counter_values_gathered[c]   = t;
	}
    // eliminate doubles
    int ctop=total_n_counter_values, duplicates=0;
    for (int cstart=0; cstart<ctop-2; cstart++)
      if (counter_values_gathered[cstart]==counter_values_gathered[cstart+1]) {
	for (int c=cstart; c<ctop-2; c++)
	  counter_values_gathered[c] = counter_values_gathered[c+1];
	ctop--; duplicates++;
      }
    printf("All counter values (eliminating %d doubles):",duplicates);
    for (int v=0; v<ctop; v++)
      printf(" %d",counter_values_gathered[v]);
    printf("\n");
  }
}
