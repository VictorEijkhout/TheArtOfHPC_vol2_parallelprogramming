! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2023
!****
!**** MPI Exercise to illustrate sequentialization
!**** fortran 2008 version
!****
!****************************************************************/

Program RightSend

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno
  integer :: source,target, error,errors

  !! data for this exercise:
  real*8 :: mydata, & !! my data that will be sent right
       leftdata=0,  & !! container for data received from left
       check
  integer :: sendto,recvfrom
  type(MPI_Request) :: requests(4)
 
  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  mydata = procno

  !! Exercise:
  !! set `sendto' and `recvfrom'

  !!solution
  sendto = procno+1
  if (procno==nprocs-1) sendto = MPI_PROC_NULL
  recvfrom = procno-1
  if (procno==0) recvfrom = MPI_PROC_NULL
  !!solved

  !!
  !! Exercise 1:
  !! -- do the Send/Recv calls
  !! Exercise 2:
  !! use MPI_Sendrecv
  !!

  !!solution
  call MPI_Recv(leftdata,1,MPI_REAL8,recvfrom, &
       0,comm,MPI_STATUS_IGNORE)
  call MPI_Send(mydata,1,MPI_REAL8,sendto, &
       0,comm)
  !!solved

  !!
  !! Check correctness
  !!
  if (procno.gt.0 .and. leftdata/=mydata-1) then
     error = procno
  else
     error = nprocs
  end if
  call MPI_Allreduce(error,errors,1,MPI_INTEGER,MPI_MIN,comm)
  if (procno.eq.0) then
     if (errors.eq.nprocs) then
        print *,"Finished; all results correct"
     else
        print *,"First error occurred on proc",errors
     end if
  end if

  call MPI_Finalize()
  
end Program RightSend

