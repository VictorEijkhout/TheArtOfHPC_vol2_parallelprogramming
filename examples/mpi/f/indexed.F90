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

Program Indexed

  use mpi
  implicit none

  integer :: newvectortype;
  integer,dimension(:),allocatable :: indices,blocklengths,&
       source,targt
  integer :: sender = 0, receiver = 1, count = 5,totalcount = 15
  integer :: recv_status(MPI_STATUS_SIZE),recv_count

#include "globalinit.F90"

  if (ntids<2) then
    print *,"This program needs at least two processes"
    stop
  end if

  ALLOCATE(indices(count))
  ALLOCATE(blocklengths(count))
  ALLOCATE(source(totalcount))
  ALLOCATE(targt(count))

  indices(0) = 2; indices(1) = 3; indices(2) = 5;
  indices(3) = 7; indices(4) = 11;
  do i=1,count
     blocklengths(i) = 1
  end do
  do i=1,totalcount
     source(i) = i
  end do

  if (mytid==sender) then
     call MPI_Type_indexed(count,blocklengths,indices,MPI_INT,&
          newvectortype,err)
     call MPI_Type_commit(newvectortype,err)
     call MPI_Send(source,1,newvectortype,receiver,0,comm,err)
     call MPI_Type_free(newvectortype,err)
  else if (mytid==receiver) then
    call MPI_Recv(targt,count,MPI_INT,sender,0,comm,&
         recv_status,err)
    call MPI_Get_count(recv_status,MPI_INT,recv_count,err)
    !    ASSERT(recv_count==count);
 end if
  
  ! if (mytid==receiver) {
  !   int i=3,val=7;
  !   if (targt(i)!=val)
  !     printf("location %d %d s/b %d\n",i,targt(i),val);
  !   i=4; val=11;
  !   if (targt(i)!=val)
  !     printf("location %d %d s/b %d\n",i,targt(i),val);
  ! }

 call MPI_Finalize(err)

end Program Indexed

