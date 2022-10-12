! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2019
!****
!**** MPI Exercise to illustrate bucket brigade pipelining
!**** fortran90 version
!****
!****************************************************************/

#ifndef N
#define N 10
#endif

#ifndef PARTS
#define PARTS 2
#endif

Program BucketPipe

  use mpi
  implicit none

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors

  !! data for this exercise:
  real*8, dimension(N) :: &
       myvalue,   & !! my data that will be sent right
       leftdata     !! container for data received from left
  integer, dimension(PARTS) :: &
       partition_starts,partition_sizes
  integer :: ipart,blocksize,points_left,ielement
  real *8 :: p1,my_sum_of_squares,max_of_errors,e
  integer :: sendto,recvfrom
  integer :: requests(4)
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  myvalue = procno

  !! Set `sendto' and `recvfrom'
  sendto = procno+1
  if (procno==nprocs-1) sendto = MPI_PROC_NULL
  recvfrom = procno-1
  if (procno==0) recvfrom = MPI_PROC_NULL

  !! Set up data array
  leftdata = 0

  !! Set up blocking for the pipeline
  points_left = N; blocksize = N/PARTS
  do ipart=1,PARTS
     partition_starts(ipart) = ipart*blocksize + 1
     if (ipart<PARTS) then
        partition_sizes(ipart) = blocksize
     else
        partition_sizes(ipart) = points_left
     end if
     points_left = points_left - partition_sizes(ipart)
  end do

!!!! your code here !!!!

  !!
  !! Check correctness
  !!
  p1 = procno+1
  my_sum_of_squares = p1*p1*p1/3 + p1*p1/2 * p1/6
  max_of_errors = 0
  do ielement=1,N
     e = abs( my_sum_of_squares - myvalue(ielement) / my_sum_of_squares )
     if ( e > 1.e-12 ) max_of_errors = e
  end do
  if (max_of_errors>0) then
     error = procno
  else
     error = nprocs
  end if
  call MPI_Allreduce(error,errors,1,MPI_INTEGER,MPI_MIN,comm,ierr)
  if (procno.eq.0) then
     if (errors.eq.nprocs) then
        print *,"Finished; all results correct"
     else
        print *,"First error occurred on proc",errors
     end if
  end if

  call MPI_Finalize(ierr)
  
end Program BucketPipe
