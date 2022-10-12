! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-6
!****
!**** MPI Exercise 
!****
!****************************************************************/

Program PingPong
  implicit none

#include <mpif.h>

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: processA,processB
  double precision,dimension(:) :: senddata(10000),recvdata(10000)
  integer :: nexperiments = 10000,iexperiment,nanosec
  double precision :: t,s=1.

  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  
  !! Exercise:
  !! -- set source and target processors two ways:
  !!    close together and far apart
  !! -- run the experiment both ways.
!!!! your code here !!!!
  if (procno==processA) then
     t = MPI_Wtime()
     do iexperiment=1,nexperiments
        senddata(1) = s
        call MPI_Send(senddata,1,MPI_DOUBLE_PRECISION, &
             !! fill in dest and tag
!!!! your code here !!!!
             comm,ierr)
        call MPI_Recv(recvdata,1,MPI_DOUBLE_PRECISION, &
             !! fill in source and tag
!!!! your code here !!!!
             comm,MPI_STATUS_IGNORE,ierr)
        s = s + recvdata(1)
     end do     
     t = MPI_Wtime()-t
     t = t/nexperiments
     nanosec = t * 1000000000;
     print '(a18,1x,f7.3)',"Time for pingpong:",nanosec*1.e-3;
  else if (procno==processB) then
     do iexperiment=1,nexperiments
        call MPI_Recv(recvdata,1,MPI_DOUBLE_PRECISION, &
             !! fill in source and tag
!!!! your code here !!!!
             comm,MPI_STATUS_IGNORE,ierr)
        call MPI_Send(recvdata,1,MPI_DOUBLE_PRECISION, &
             !! fill in dest and tag
!!!! your code here !!!!
             comm,ierr)
     end do     
  end if
  
  call MPI_Finalize(ierr)
  
end Program PingPong
