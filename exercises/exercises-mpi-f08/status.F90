! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for MPI_Status use
!**** fortran 2008 version
!****
!****************************************************************/

Program Status

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  type(MPI_Status) :: recv_status
  integer :: nprocs, procno,ierr
  integer :: processA,processB

  ! buffers for ping and pong
  integer,parameter :: buffersize = 1000
  integer :: randomsize,i
  double precision,dimension(:) :: &
       senddata(buffersize),recvdata(buffersize)
  double precision :: s=1.

  ! random stuff
  integer,allocatable,dimension(:) :: randseed
  integer :: randsize
  real(4) :: randomfraction

  !!
  !! Initialize the random number generator
  !! using the process number to get a unique seed
  !!
  call random_seed(size=randsize)
  allocate(randseed(randsize))
  do i=1,randsize
     randseed(i) = 1023*procno
  end do
  call random_seed(put=randseed)
  
  !!
  !! Setup MPI
  !!
  call MPI_Init()
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  
  !! Exercise:
  !! -- set source and target processors two ways:
  !!    close together and far apart
  !! -- run the experiment both ways.
!!!! your code here !!!!
  if (procno==processA) then
     call random_number(randomfraction)
     randomsize = randomfraction * buffersize
     print *,"Sending elements:",randomsize
     do i=1,randomsize
        senddata(i) = i
     end do
     call MPI_Send(senddata,randomsize,MPI_DOUBLE_PRECISION, &
          !! fill in dest and tag
!!!! your code here !!!!
          comm)
     call MPI_Recv(recvdata,1,MPI_DOUBLE_PRECISION, &
          !! fill in source and tag
!!!! your code here !!!!
          comm,MPI_STATUS_IGNORE)
  else if (procno==processB) then
     call MPI_Recv(recvdata, &
          !!
          !! Exercise:
          !! - specify a large enough buffer
          !!   no matter how much is actually sent.
          !!
!!!! your code here !!!!
          processA,0,comm,recv_status)
     call MPI_Send(recvdata,1,MPI_DOUBLE_PRECISION, &
          processA,0, comm)
     !!
     !! Exercise:
     !! - use MPI_SOURCE to print where the message came from
     !! - use MPI_Get_count to find out how many elements were received
     !!
!!!! your code here !!!!
     print *,"Received elements:",randomsize
  end if
  
  call MPI_Finalize()
  
end Program Status
