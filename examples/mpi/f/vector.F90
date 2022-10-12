! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-5
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Vector

  use mpi
  implicit none

  double precision, dimension(:),allocatable :: source,target
  integer :: sender = 0,receiver = 1, count = 5, stride = 2

  integer :: newvectortype
  integer :: recv_status(MPI_STATUS_SIZE),recv_count

#include "globalinit.F90"

  if (ntids<2) then
    print *,"This program needs at least two processes"
    stop
  end if

  ALLOCATE(source(stride*count))
  ALLOCATE(target(stride*count))

  do i=1,stride*count
     source(i) = i+.5;
  end do

  if (mytid==sender) then
     call MPI_Type_vector(count,1,stride,MPI_DOUBLE_PRECISION,&
          newvectortype,err)
     call MPI_Type_commit(newvectortype,err)
     call MPI_Send(source,1,newvectortype,receiver,0,comm,err)
     call MPI_Type_free(newvectortype,err)
  else if (mytid==receiver) then
     call MPI_Recv(target,count,MPI_DOUBLE_PRECISION,sender,0,comm,&
          recv_status,err)
     call MPI_Get_count(recv_status,MPI_DOUBLE_PRECISION,recv_count,err)
  end if
  
  if (mytid==receiver) then
    ! for (i=0; i<count; i++)
    !   if (target[i]!=source[stride*i])
    !     printf("location %d %e s/b %e\n",i,target[i],source[stride*i]);
  end if

  call MPI_Finalize(err)

end Program Vector
