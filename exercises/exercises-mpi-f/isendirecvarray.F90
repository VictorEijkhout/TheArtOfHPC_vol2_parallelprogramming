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
!****
!****************************************************************/

Program IsendIrecvArray
  implicit none

#include <mpif.h>

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target

  !! data for this exercise:
#define N 100
  real(8) :: indata(N),outdata(N)
  real(8) :: leftdata=0.d0, rightdata=0.d0
  integer :: sendto,recvfrom,i
  integer :: requests(4)
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  
  forall (i=1:N) indata(i) = 1.
  
  !! Exercise:
  !! set `sendto' and `recvfrom' twice:
  !! once to get data from the left, once from the right

  !!
  !! first get left neighbor data
  !!

  !! specify processors:
!!!! your code here !!!!

  !! now send to the right / receive from the left
  call MPI_Isend( &
!!!! your code here !!!!
       1,MPI_DOUBLE, sendto,0, comm, &
!!!! your code here !!!!
       ierr)
  call MPI_Irecv(leftdata, 1,MPI_DOUBLE, recvfrom,0, comm, &
!!!! your code here !!!!
       ierr)

  !!
  !! then get right neighbor data
  !!

  !! specify processors:
!!!! your code here !!!!
!!!! your code here !!!!
       1,MPI_DOUBLE, sendto,0, comm, &
!!!! your code here !!!!
       ierr)
  call MPI_Irecv(rightdata, 1,MPI_DOUBLE, recvfrom,0, comm, &
!!!! your code here !!!!
       ierr)

  !!
  !! Now make sure all Irecv/Irecv operations are completed
  !!
!!!! your code here !!!!

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
  !! Check correctness
  !!
  do i=1,N
     if ( (procno==0 .and. i==1) .or. ( procno==nprocs-1 .and. i==N) ) then
        if (outdata(i)/=2) &
             print *,"Data on process",procno,", index",i," should be 2, not",outdata(i)
     else
        if (outdata(i)/=3) &
             print *,"Data on process",procno,", index",i," should be 3, not",outdata(i)
     end if
  end do
  if (procno==0) then
     print *,"Finished"
  end if
  
  call MPI_Finalize(ierr)
  
end Program IsendIrecvArray

