! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2021
!****
!**** MPI Exercise to illustrate pipelining
!****
!****************************************************************/

Program Setdifference

#ifndef N
#define N 10
#endif

  use mpi_f08
  implicit none

  integer :: nprocs, procno,sendto,recvfrom,proc,i
  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer,dimension(N) :: mydata,filter,result
  ! random
  integer :: seedsize
  integer,dimension(:),allocatable :: seed
  real*4 :: r

  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  !!
  !! Initialize local data and filter
  !!
  call random_seed(size=seedsize) 
  allocate(seed(seedsize))
  seed(:) = procno*nprocs
  call random_seed(put=seed)
  do i=1,N
     call random_number(r)
     mydata(i) = r*(2*N*nprocs); result(i) = mydata(i)
     call random_number(r)
     filter(i) = r*(2*N*nprocs);
  end do
  if (procno==0) &
     print *,"Initial data:"
  call print_distarray(mydata,comm,.false.)
  if (procno==0) &
     print *,"Filter:"
  call print_distarray(filter,comm,.true.)

  !!
  !! Exercise:
  !! -- realize the bucket brigade algorithm
  !!    receive a next bit of the filter,
  !!    as many times as there are processes
  !! -- and then take the difference of your local result
  !!    with the new filter bit
  !!
  sendto = mod( procno+1,nprocs )
  recvfrom = mod( procno-1+nprocs,nprocs )
  do proc=1,nprocs
!!!! your code here !!!!
     call setdiff( result,filter )
!!!! your code here !!!!
  end do

  !!
  !! Printing to screen is the only correctness check right now
  !!
  if (procno==0) &
     print *,"Final:"
  call print_distarray(result,comm,.false.)

contains
  !!
  !! Compare local data `mydata' against incoming `filter'
  !! Set local data to negative if it occurs in the incoming.
  !!
  subroutine setdiff(mydata,filter)
    implicit none
    integer,dimension(:),intent(inout) :: mydata
    integer,dimension(:),intent(in) :: filter
    integer :: n,i_my,i_ot,my,ot
    n = size(mydata)
    do i_my=1,n
       my = mydata(i_my)
       do i_ot=1,n
          ot = filter(i_ot)
          if (my==ot) then
             mydata(i_my) = -1; exit
          end if
       end do
    end do
  end subroutine setdiff

  subroutine print_distarray(local,comm,sort)
    implicit none
    ! parameters
    integer,dimension(:),intent(in) :: local
    Type(MPI_Comm),intent(in) :: comm
    logical,intent(in) :: sort
    ! local
    integer,dimension(:),allocatable :: global
    integer :: n,nprocs,procno, top,s,tmp

    n = size(local)
    call MPI_Comm_size(comm,nprocs)
    call MPI_Comm_rank(comm,procno)
    if (procno==0) Allocate( global(n*nprocs) )
    call MPI_Gather( local,n,MPI_INTEGER,global,n,MPI_INTEGER,0,comm)
    if (procno==0) then
       if (sort) then
          do top=nprocs*n,1,-1
             do s=1,top-1
                if (global(s+1)<global(s)) then
                   tmp = global(s+1)
                   global(s+1) = global(s)
                   global(s) = tmp
                end if
             end do
          end do
       end if
       print '(100(1x,i3))',(global(s),s=1,n*nprocs)
    end if
  end subroutine print_distarray

end Program Setdifference
