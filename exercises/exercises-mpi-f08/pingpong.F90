! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2023
!****
!**** MPI Exercise for pingpong
!**** fortran 2008 version
!****
!****************************************************************/

Program PingPong

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno
  integer :: processA,processB
  double precision,dimension(:) :: senddata(10000),recvdata(10000)
  integer :: nexperiments = 100000,iexperiment,nanosec
  double precision :: t,s=1.

  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  
  !! Exercise:
  !! -- set source and target processors two ways:
  !!    close together and far apart
  !! -- run the experiment both ways.
  !!solution
  processA = 0
  processB = nprocs-1
  !!solved
  if (procno==processA) then
     t = MPI_Wtime()
     do iexperiment=1,nexperiments
        senddata(1) = s
        call MPI_Send(senddata,1,MPI_DOUBLE_PRECISION, &
             !! fill in dest and tag
             !!solution
             processB,0, &
             !!solved
             comm)
        call MPI_Recv(recvdata,1,MPI_DOUBLE_PRECISION, &
             !! fill in source and tag
             !!solution
             processB,0, &
             !!solved
             comm,MPI_STATUS_IGNORE)
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
             !!solution
             processA,0,&
             !!solved
             comm,MPI_STATUS_IGNORE)
        call MPI_Send(recvdata,1,MPI_DOUBLE_PRECISION, &
             !! fill in dest and tag
             !!solution
             processA,0, &
             !!solved
             comm)
     end do     
  end if
  
  call MPI_Finalize()
  
end Program PingPong
