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

Program Isendirecv
  use tools
  use mpi
  implicit none

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors
  logical :: error_condition

  !! data for this exercise:
  real*8 :: mydata, leftdata=0, rightdata=0, check
  integer :: sendto,recvfrom
  integer :: requests(4)
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  mydata = procno

  !! Exercise:
  !! set `sendto' and `recvfrom' twice:
  !! once to get data from the left, once from the right

  !!
  !! first get left neighbor data
  !!

  !! set processes to communicate with
!!!! your code here !!!!

  !! now do the Isend/Irecv calls
  call MPI_Isend(mydata, 1,MPI_REAL8, sendto,0, comm, &
!!!! your code here !!!!
       ierr)
  call MPI_Irecv(leftdata, 1,MPI_REAL8, recvfrom,0, comm, &
!!!! your code here !!!!
       ierr)

  !!
  !! then get right neighbor data
  !!

  !! set processors to communicate with
!!!! your code here !!!!

  !! now do the Isend/Irecv calls
  call MPI_Isend(mydata, 1,MPI_REAL8, sendto,0, comm, &
!!!! your code here !!!!
       ierr)
  call MPI_Irecv(rightdata, 1,MPI_REAL8, recvfrom,0, comm, &
!!!! your code here !!!!
       ierr)

  !!
  !! Now make sure all Irecv/Irecv operations are completed
  !!
!!!! your code here !!!!

  !!
  !! Check correctness
  !!
  mydata = mydata+leftdata+rightdata

  if (procno==0) then
     check = 2*procno+1
  else if (procno==nprocs-1) then
     check = 2*procno-1
  else
     check = 3*procno
  end if

  error = nprocs;
  error_condition = .not. ISAPPROX(mydata,check)
  call print_final_result(error_condition,comm)

  call MPI_Finalize(ierr)
  
contains
  logical function ISAPPROX(x,y)
    real*8,intent(in) :: x,y
    isapprox = ( x==0. .and. abs(y)<1.e-14 ) &
         .or. ( y==0. .and. abs(x)<1.e-14 ) &
         .or. abs(x-y)/abs(x)<1.e-14
  end function ISAPPROX
  
end Program Isendirecv

