! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2020
!****
!**** MPI Exercise 
!****
!****************************************************************/

Program SumSquares
  implicit none

#include <mpif.h>

  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  !! data specific for this program
  integer :: nlocal,nglobal,i,itmp
  real(8),dimension(:),allocatable :: local_squares
  real(8) :: local_sum,global_sum

  call MPI_Init(ierr)
  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  nlocal = 1000
  nglobal = nprocs*nlocal
  Allocate(local_squares(nlocal))

  !!
  !! Exercise part 1:
  !! -- set your local values
  !!
  do i=1,nlocal
!!!! your code here !!!!
  end do

  !!
  !! Exercise part 2:
  !! -- do the local summation
  !! -- do the global summation
  !!
  local_sum = 0.d0
  do i=1,nlocal
!!!! your code here !!!!
  end do
  call MPI_Reduce(local_sum,global_sum,1,MPI_REAL8,&
       !! operator:
!!!! your code here !!!!
       0,comm,ierr)
  if (procno==0) &
       print *,"Global sum=",global_sum," should be",&
       (2.d0*nglobal*nglobal*nglobal + 3.*nglobal*nglobal + nglobal)/6.d0

  call MPI_Finalize(ierr)
  
end Program SumSquares
