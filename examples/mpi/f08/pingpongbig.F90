! /****************************************************************
!  ****
!  **** This program file is part of the book 
!  **** `Parallel programming for Science and Engineering'
!  **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!  ****
!  **** copyright Victor Eijkhout 2012-2022
!  ****
!  **** pingpong with big counts
!  ****
!  ****************************************************************/

Program PingPong

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: processA,processB
  double precision,dimension(:),allocatable :: senddata,recvdata
  integer :: nexperiments = 1,iexperiment
  double precision :: t,s=1.,nanosec
  !! specific for this code
  !!codesnippet bigdataf
  integer :: power,countbytes
  Integer(KIND=MPI_COUNT_KIND) :: length
  !!codesnippet end

  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  
  processA = 0
  processB = nprocs-1
  !!codesnippet bigdataf
  call MPI_Sizeof(length,countbytes,ierr)
  if (procno==0) &
       print *,"Bytes in count:",countbytes
  !!codesnippet end

  powerloop:  do power=3,10
     if (procno==processA .or. procno==processB) then
  !!codesnippet bigdataf
        length = 10**power
        allocate( senddata(length),recvdata(length) )
  !!codesnippet end
     end if
     if (procno==processA) then
        t = MPI_Wtime()
        do iexperiment=1,nexperiments
           senddata(1) = s
  !!codesnippet bigdataf
           call MPI_Send(senddata,length,MPI_DOUBLE_PRECISION, &
                processB,0, comm)
           call MPI_Recv(recvdata,length,MPI_DOUBLE_PRECISION, &
                processB,0, comm,MPI_STATUS_IGNORE)
  !!codesnippet end
           s = s + recvdata(1)
        end do
        t = MPI_Wtime()-t
        t = t/nexperiments
        nanosec = t * 1000000000;
        print '(a26,1x,f9.1)',"Time for pingpong in msec:",nanosec*1.e-6;
     else if (procno==processB) then
        do iexperiment=1,nexperiments
           call MPI_Recv(recvdata,length,MPI_DOUBLE_PRECISION, &
                processA,0, comm,MPI_STATUS_IGNORE)
           call MPI_Send(recvdata,length,MPI_DOUBLE_PRECISION, &
                processA,0, comm)
        end do
     end if
     if (procno==processA .or. procno==processB) then
        deallocate( senddata,recvdata )
     end if
  end do powerloop

  call MPI_Finalize()
  
end Program PingPong
