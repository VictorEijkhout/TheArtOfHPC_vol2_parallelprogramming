! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for MPI_Reduce
!**** fortran 2008 version
!****
!****************************************************************/

Program RandomMax

  use mpi_f08
  use tools
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno, ierr,i
  logical :: error

  ! stuff for the random number generator
  integer :: randomint,sender
  integer :: randsize
  integer,allocatable,dimension(:) :: randseed
  real(8) :: my_random,scaled_random,sum_random,sum_scaled_random

  call MPI_Init(); 
  call MPI_Comm_rank(comm,procno); 
  call MPI_Comm_size(comm,nprocs); 

  !!
  !! Initialize the random number generator
  !! using the process number to get a unique seed
  !!
  call random_seed(size=randsize)
  allocate(randseed(randsize))
  do i=1,randsize
     randseed(i) = 1023+procno
  end do
  call random_seed(put=randseed)

  !! My own random
  call random_number(my_random)
  print '(("Process",i4,x,"has random value",x,f9.7))', procno,my_random

  !!
  !! Exercise:
  !! -- compute the sum of the values, everywhere
  !! -- scale your number by that sum
  !! -- check that the sum of scaled values is 1
  !!
  call MPI_Allreduce( &
!!!! your code here !!!!
       comm)
  scaled_random = my_random / sum_random
  call MPI_Allreduce( &
!!!! your code here !!!!
       comm)

  !!
  !! Correctness test
  !!
  error = .false.
  if (abs(sum_scaled_random-1.)>1.e-12) then
     print *,"Suspicious sum",sum_scaled_random,"on rank",procno
     error = .true.
  end if
  call print_final_result(error,comm)
  
  call MPI_Finalize()
  
end Program RandomMax

