! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-6
!****
!**** MPI Exercise 
!****
!****************************************************************/

Program CommRank

  use mpi
  implicit none

  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  call MPI_Init(ierr)

  !! Exercise part 1:
  !! -- Use the routine
  !!    MPI_Comm_size and MPI_Comm_rank
  !! -- Let each processor print out a message like
  !!    "Hello from processor 7 out of 12"
  !!    reporting its number and the total number.
!!!! your code here !!!!
  
  !! Exercise part 2:
  !! -- let only processs zero print out
  !!    "There are 16 processes"
  !!    reporting only the total number
!!!! your code here !!!!
  
  call MPI_Finalize(ierr)
  
end Program CommRank
