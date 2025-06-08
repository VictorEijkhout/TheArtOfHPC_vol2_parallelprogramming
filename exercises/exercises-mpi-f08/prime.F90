! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2023
!****
!**** MPI Exercise 
!**** fortran 2008 version
!****
!****************************************************************/

Program Prime

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno
  integer :: bignum = 2000000111, maxfactor = 45200
  !! some more variables for this exercise
  integer :: myfactor, &
  !!solution
       mylow,myhi
  !!solved
  
  call MPI_Init()

  !! Exercise:
  !! -- Parallelize the do loop so that each processor
  !!    tries different candidate numbers.
  !! -- If a processors finds a factor, print it to the screen.

  !! insert loop header:
  !!solution
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  mylow = procno*(maxfactor/nprocs)
  myhi = (procno+1)*(maxfactor/nprocs)
  if (mylow<2) mylow = 3
  if (MOD(mylow,2)==0) mylow = mylow+1
  !!solved
  do &
       !!solution
       myfactor=mylow,myhi,2
     !!solved
     if (MOD(bignum,myfactor)==0) &
          print *,"Processor",procno,"found factor",myfactor
  end do
  
  call MPI_Finalize()
  
end Program Prime
