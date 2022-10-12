! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for user-defined operators
!**** fortran 90/95 version
!****
!****************************************************************/

Program OneNorm

  use mpi
  implicit none

  integer :: nprocs, procno,ierr
  integer :: comm

  !! data specific for this program
  integer :: calc_one
  integer :: nlocal,nglobal,i,itmp
  real :: data, data_one_norm, actual_norm

  call MPI_Init(ierr)
  comm = MPI_COMM_WORLD
  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  data = -1.e0

  call MPI_Op_create(one_norm,.true.,calc_one,ierr)
  call MPI_Allreduce(data,data_one_norm,1,MPI_REAL,calc_one,comm,ierr)
  actual_norm = nprocs
  if (data_one_norm/=actual_norm) then
     print *,procno,": result",data_one_norm," should be",actual_norm
  end if

  call MPI_Finalize(ierr)
  
contains
  integer function one_norm(invec,inoutvec,len,type)
    implicit none
    ! parameters
    Integer,intent(in) :: len
    Real,intent(in),dimension(len) :: invec
    Real,intent(inout),dimension(len) :: inoutvec
    integer,intent(in) :: type
    ! local variables
    Real :: m,n,r

    n = invec(1); r = inoutvec(1)
    !!
    !! Exercise:
    !! -- do the reduction step for a 1-norm
    !!
!!!! your code here !!!!
    inoutvec(1) = m
    one_norm = 0
  end function one_norm
end Program OneNorm
