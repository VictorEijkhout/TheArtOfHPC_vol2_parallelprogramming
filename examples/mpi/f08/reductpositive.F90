!   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   %%%%
!   %%%% This program file is part of the book and course
!   %%%% "Parallel Computing"
!   %%%% by Victor Eijkhout, copyright 2013-8
!   %%%%
!   %%%% reductpositive.c : user-defined reduction
!   %%%%
!   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program ReductPositive

  use mpi_f08

  implicit none

  integer,parameter :: ndata = 10
  integer,dimension(ndata) :: data = [ 2,3,0,5,0,1,8,12,4,0 ]
  integer :: m,mreduct = 2000000000, positive_minimum,i
  !!codesnippet opcreatef
  Type(MPI_Op) :: rwz
  !!codesnippet end

#include "globalinit.F90"

  if (nprocs>ndata) then
     print *,"Too many procs for this example: at most ",ndata
     stop 1
  end if
    
  do i=1,nprocs
     if (data[i]<mreduct .and. data[i]>0) &
          mreduct = data(i)
  end do

  !!codesnippet opcreatef
  call MPI_Op_create(reduce_without_zero,1,rwz);
  MPI_Allreduce(data+procno,&positive_minimum,1,MPI_INT,rwz,comm);
  !!codesnippet end

  !! check that the distributed result is the same as sequential
  if (mreduct!=positive_minimum)
    printf("[%d] Result %d should be %d\n",
           procno,positive_minimum,mreduct);
  else if (procno==0)
    printf("User-defined reduction successful: %d\n",positive_minimum);

  MPI_Finalize();
  return 0;
}

contains

!!codesnippet mpirwzf
void reduce_without_zero(void *in,void *inout,int *len,MPI_Datatype *type) {
  !! r is the already reduced value, n is the new value
  int n = *(int*)in, r = *(int*)inout;
  int m;
  if (n==0) { !! new value is zero: keep r
    m = r;
  } else if (r==0) {
    m = n;
  } else if (n<r) { !! new value is less but not zero: use n
    m = n;
  } else { !! new value is more: use r
    m = r;
  };
  !!codesnippet end
#ifdef DEBUG
  printf("combine %d %d : %d\n",r,n,m);
#endif
  !! return the new value
  !!codesnippet mpirwz
  *(int*)inout = m;  
}
!!codesnippet end

End Program ReductPositive

