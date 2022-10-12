! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise to illustrate sequentialization
!**** fortran90 version
!****
!****************************************************************/

Program RightSend

  use mpi
  implicit none

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors

  !! data for this exercise:
  real*8 :: mydata, & !! my data that will be sent right
       leftdata=0,  & !! container for data received from left
       check
  integer :: sendto,recvfrom
  integer :: requests(4)
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  mydata = procno

  !! Exercise:
  !! set `sendto' and `recvfrom'

!!!! your code here !!!!

  !!
  !! Exercise 1:
  !! -- do the Send/Recv calls
  !! Exercise 2:
  !! use MPI_Sendrecv
  !!

!!!! your code here !!!!

  !!
  !! Check correctness
  !!
  if (procno.gt.0 .and. leftdata/=mydata-1) then
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
  
end Program RightSend

