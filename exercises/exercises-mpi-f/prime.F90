! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-7
!****
!**** MPI Exercise 
!****
!****************************************************************/

Program Prime
  implicit none

#include <mpif.h>

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: bignum = 2000000111, maxfactor = 45200
  !! some more variables for this exercise
  integer :: myfactor, &
!!!! your code here !!!!
  
  call MPI_Init(ierr)

  !! Exercise:
  !! -- Parallelize the do loop so that each processor
  !!    tries different candidate numbers.
  !! -- If a processors finds a factor, print it to the screen.

  !! insert loop header:
!!!! your code here !!!!
  do &
!!!! your code here !!!!
     if (MOD(bignum,myfactor)==0) &
          print *,"Processor",procno,"found factor",myfactor
  end do
  
  call MPI_Finalize(ierr)
  
end Program Prime
