! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for Ibarrier
!**** fortran 2008 version
!****
!****************************************************************/

Program Isendirecv

  use mpi
  implicit none

  Integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,tag,target

  !! random stuff
  integer,allocatable,dimension(:) :: randseed
  integer :: randsize
  real(4) :: randomfraction

  !! data for this exercise:
  real*8 :: receive_data
  real*8,dimension(:),allocatable :: send_data
  integer :: n_destinations,idestination,destination,step
  logical :: all_done_flag
  Integer :: final_barrier
  Integer,dimension(:),allocatable :: send_requests
  logical :: message_flag
  integer :: recv_status(MPI_STATUS_SIZE)
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! How many processes are we going to send to?
  !!
  call random_number(randomfraction)
  n_destinations = nprocs * randomfraction
  ALLOCATE(send_requests(n_destinations))
  ALLOCATE(send_data(n_destinations))
  !!
  !! Pick random processes to send to, and post an Isend to them
  !!
  do idestination=1,n_destinations
     call random_number(randomfraction)
     destination = nprocs * randomfraction
     print '("[",i3,"] send to",i3)',procno,destination
     call MPI_Isend(send_data(idestination+1),1,MPI_REAL8,&
          destination,0,comm,send_requests(idestination+1),ierr)
  end do

  !!
  !! When you're done sending, post a non-blocking barrier
  !!
  print '("[",i3,"] posting barrier")',procno
  call MPI_Ibarrier(comm,final_barrier,ierr)

  step = 0
  DO
     step = step+1
     !!
     !! Exercise part 1:
     !! -- use MPI_Test to determine when the barrier is complete;
     !!    in that case you quit
     !!
     all_done_flag = .FALSE.
     call MPI_Test(final_barrier,all_done_flag,MPI_STATUS_IGNORE,ierr)
!!!! your code here !!!!

     !!
     !! Exercise part 2:
     !! -- use MPI_Iprobe to test if there is a message
     !! part 1: if there is not message, skip to the next iteration
     !!
     message_flag = .FALSE.
     call MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,comm,message_flag,recv_status,ierr)
!!!! your code here !!!!
     !!
     !! part 2: the message can come from anywhere, so
     !!    you need to inspect the status to find the source and tag
     !!
!!!! your code here !!!!
     call MPI_Recv(receive_data,1,MPI_REAL8,&
          source,MPI_ANY_TAG,comm,MPI_STATUS_IGNORE,ierr)
     print '("[",i3,"] received from ",i3)',procno,source
  END DO
  print '("[",i3,"] concluded after ",i3," steps")',procno,step

  call MPI_Finalize(ierr)
  
end Program Isendirecv

