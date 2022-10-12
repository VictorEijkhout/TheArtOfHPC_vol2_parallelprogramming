!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2020
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Contiguous

  use mpi
  implicit none

  integer :: sender = 0, receiver = 1, count = 5
  double precision, dimension(:),allocatable :: source,target

  integer :: newvectortype
  integer :: recv_status(MPI_STATUS_SIZE),recv_count

#include "globalinit.F90"

  if (ntids<2) then
    print *,"This program needs at least two processes"
    stop
  end if

  ALLOCATE(source(count))
  ALLOCATE(target(count))

  do i=1,count
     source(i) = i+.5;
  end do

  if (mytid==sender) then
     call MPI_Type_contiguous(count,MPI_DOUBLE_PRECISION,newvectortype,err)
     call MPI_Type_commit(newvectortype,err)
     call MPI_Send(source,1,newvectortype,receiver,0,comm,err)
     call MPI_Type_free(newvectortype,err)
  else if (mytid==receiver) then
     call MPI_Recv(target,count,MPI_DOUBLE_PRECISION,sender,0,comm,&
          recv_status,err)
     call MPI_Get_count(recv_status,MPI_DOUBLE_PRECISION,recv_count,err)
     !ASSERT(count==recv_count);
  end if
  
  if (mytid==receiver) then
    ! for (i=0; i<count; i++)
    !   if (target[i]!=source[i])
    !     printf("location %d %e s/b %e\n",i,target[i],source[i]);
  end if

  call MPI_Finalize(err)

end Program Contiguous

