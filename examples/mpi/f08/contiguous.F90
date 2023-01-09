!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Contiguous

  use mpi_f08
  implicit none

  integer :: sender = 0, receiver = 1, count = 5
  double precision, dimension(:),allocatable :: source,target

!codesnippet contiguous-f08
  integer :: newvectortype
!codesnippet end
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

!codesnippet contiguous-f08
  if (mytid==sender) then
     call MPI_Type_contiguous(count,MPI_DOUBLE_PRECISION,newvectortype)
     call MPI_Type_commit(newvectortype)
     call MPI_Send(source,1,newvectortype,receiver,0,comm)
     call MPI_Type_free(newvectortype)
  else if (mytid==receiver) then
     call MPI_Recv(target,count,MPI_DOUBLE_PRECISION,sender,0,comm,&
          recv_status)
     call MPI_Get_count(recv_status,MPI_DOUBLE_PRECISION,recv_count)
     !ASSERT(count==recv_count);
  end if
!codesnippet end
  
  if (mytid==receiver) then
    ! for (i=0; i<count; i++)
    !   if (target[i]!=source[i])
    !     printf("location %d %e s/b %e\n",i,target[i],source[i]);
  end if

  call MPI_Finalize(err)

end Program Contiguous

