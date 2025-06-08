! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2022
!****
!**** MPI Exercise for Isend/Irecv
!**** fortran 2008 version
!****
!****************************************************************/

Program IsendIrecvArray
  use tools
  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno
  integer :: source,target
  logical :: error_condition

  !! data for this exercise:
#define N 100
  real(8) :: indata(N),outdata(N),answer(N)
  real(8) :: leftdata=0.d0, rightdata=0.d0
  integer :: sendto,recvfrom,i
  type(MPI_Request) :: requests(4)
 
  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  
  forall (i=1:N) indata(i) = 1.
  
  !! Exercise:
  !! set `sendto' and `recvfrom' twice:
  !! once to get data from the left, once from the right

  !!
  !! first get left neighbor data
  !!

  !! specify processors:
  !!solution
  if (procno<nprocs-1) then
     sendto = procno+1
  else
     sendto = MPI_PROC_NULL
  end if
  if (procno>0) then
     recvfrom = procno-1
  else
     recvfrom = MPI_PROC_NULL
  end if  
  !!solved

  !! now send to the right / receive from the left
  call MPI_Isend( &
       !!solution
       indata(N), &
       !!solved
       1,MPI_DOUBLE, sendto,0, comm, &
       !!solution
       requests(1) &
       !!solved
       )
  call MPI_Irecv(leftdata, 1,MPI_DOUBLE, recvfrom,0, comm, &
       !!solution
       requests(2) &
       !!solved
       )

  !!
  !! then get right neighbor data
  !!

  !! specify processors:
  !!solution
  if (procno>0) then
     sendto = procno-1
  else
     sendto = MPI_PROC_NULL
  end if
  if (procno<nprocs-1) then
     recvfrom = procno+1
  else
     recvfrom = MPI_PROC_NULL
  end if

  !! now send to the left / receive from the right
  call MPI_Isend( &
       !!solution
       indata(1), &
       !!solved
       1,MPI_DOUBLE, sendto,0, comm, &
       !!solution
       requests(3) &
       !!solved
       )
  call MPI_Irecv(rightdata, 1,MPI_DOUBLE, recvfrom,0, comm, &
       !!solution
       requests(4) &
       !!solved
       )

  !!
  !! Now make sure all Irecv/Irecv operations are completed
  !!
  !!solution
  call MPI_Waitall(4,requests,MPI_STATUSES_IGNORE)
  !!solved

  !!
  !! Do the actual averaging operation
  !!
  do i=1,N
     if (i==1) then
        outdata(i) = leftdata + indata(i) + indata(i+1)
     else if (i==N) then
        outdata(i) = indata(i-1) + indata(i) + rightdata
     else
        outdata(i) = indata(i-1) + indata(i) + indata(i+1)
     end if     
  end do
  
  !!
  !! Check correctness of the result:
  !! value should be 2 at the end points, 3 everywhere else
  !!
  do i=1,N
     if ( (procno==0 .and. i==1) .or. ( procno==nprocs-1 .and. i==N) ) then
        answer(i) = 2.
     else
        answer(i) = 3.
     end if
  end do
  error_condition = array_error(answer,outdata)
  call print_final_result(error_condition,comm)
  
  call MPI_Finalize()
  
end Program IsendIrecvArray

