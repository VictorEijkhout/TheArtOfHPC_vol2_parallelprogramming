! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2022
!****
!**** MPI Exercise 
!****
!****************************************************************/

Program SendRecv

  use mpi
  implicit none

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors

  !! data for this exercise:
  real*8 :: mydata, leftdata=0, rightdata=0, check
  integer :: sendto,recvfrom
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  mydata = procno

  !! Exercise:
  !! set `sendto' and `recvfrom' twice:
  !! once to get data from the left, once from the right

  !! first get left neighbor data
  !!hint: sendto =
  !!hint: recvfrom =
!!!! your code here !!!!
  call MPI_Sendrecv( mydata,1,MPI_REAL8, sendto,0, &
       leftdata,1,MPI_REAL8, recvfrom,0, &
       comm,MPI_STATUS_IGNORE, ierr)

  !! then get right neighbor data
!!!! your code here !!!!
  call MPI_Sendrecv( mydata,1,MPI_REAL8, sendto,0, &
       rightdata,1,MPI_REAL8, recvfrom,0, &
       comm,MPI_STATUS_IGNORE,ierr)

  !! check correctness
  mydata = mydata+leftdata+rightdata
  
  if (procno==0) then
     check = 2*procno+1
  else if (procno==nprocs-1) then
     check = 2*procno-1
  else
     check = 3*procno
  end if

  error = nprocs;
  if ( .not. ISAPPROX(mydata,check) ) then
     print *,"Data on process",procno,"should be",check," not",mydata
     error = procno
  end if
  call MPI_Allreduce(error,errors,1,MPI_INTEGER,MPI_MIN,comm,ierr)
  if (procno==0) then
     if (errors==nprocs) then
        print *,"Finished; all results correct"
     else
        print *,"First error occurred on proc",errors
     end if
  end if

  call MPI_Finalize(ierr)
  
contains
  logical function ISAPPROX(x,y)
    real*8,intent(in) :: x,y
    isapprox = ( x==0. .and. abs(y)<1.e-14 ) &
         .or. ( y==0. .and. abs(x)<1.e-14 ) &
         .or. abs(x-y)/abs(x)<1.e-14
  end function ISAPPROX
  
end Program SendRecv
