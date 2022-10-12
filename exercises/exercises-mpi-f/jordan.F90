! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-7
!****
!**** MPI Exercise for Gauss-Jordan elimination
!****
!****************************************************************/

Program GaussJordan
  implicit none

#include <mpif.h>

  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  !! stuff for the random number generator
  integer :: randomint,sender
  integer :: randsize
  integer,allocatable,dimension(:) :: randseed
  real :: my_random,global_random, scaled_random,sum_random

  !! data for this program
  real(8),dimension(:),allocatable :: matrix,solution,rhs,scalings
  real(8) :: pivot,local_solution
  integer :: row,pivot_column,n,i
  logical :: success = .true.

  call MPI_Init(ierr)
  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Initialize the random number generator
  !! using the process number to get a unique seed
  !!
  call random_seed(size=randsize)
  allocate(randseed(randsize))
  do i=1,randsize
     randseed(i) = 1023*procno
  end do
  call random_seed(put=randseed)

  !!
  !! We set the matrix size to the number of processors
  !! Each process allocates
  !! - one column of the matrix
  !! - a scaling vector
  !! - redundantly, the solution and right-hand side
  !! (We use zero-based indexing for convenience)
  !!
  N = nprocs
  allocate(matrix(0:N-1),scalings(0:N-1),rhs(0:N-1),solution(0:N-1))

  !!
  !! Fill the matrix with random data;
  !! increase the diagonal for numerical stability
  !!
  do row=0,N-1
     call random_number(my_random)
     matrix(row) = my_random
     if (row==procno) matrix(row) = matrix(row)+.5d0
  end do

  !!
  !! Set the right-hand side to the row sums,
  !! so that the solution is identically one
  !! We use one long reduction
  !!
  call MPI_Allreduce(matrix,rhs,N,MPI_REAL8,MPI_SUM,comm,ierr)

  !!
  !! Now iterate over the columns,
  !! using the diagonal element as pivot
  !! (We use zero-based indexing for convenience)
  !!
  do pivot_column=0,N-1
     if (pivot_column==procno) then
        pivot = matrix(pivot_column)
        do row=0,N-1
           scalings(row) = matrix(row)/pivot
        end do
     end if
     !!
     !! Exercise:
     !! - make sure everyone knows the scaling factors
     !!
!!!! your code here !!!!
     !!
     !! Now update the matrix
     !! Answer for yourself: why is there no loop over the columns?
     !!
     do row=0,N-1
        if (row==pivot_column) cycle
        matrix(row) = matrix(row) - scalings(row)*matrix(pivot_column)
     end do
     !! update the rhs
     do row=0,N-1
        if (row==pivot_column) cycle
        rhs(row) = rhs(row) - scalings(row)*rhs(pivot_column)
     end do
  end do

  !!
  !! Check that our local column is swept
  !!
  do row=0,N-1
     if (row==procno) cycle
     if (abs(matrix(row))>1.d-14) &
        write(*,'("Wrong value at [",i3,",",i3,"]:",f9.5)') row,procno,matrix(row)
  end do

  !!
  !! Solve the system
  !!
  local_solution = rhs(procno)/matrix(procno)
  call MPI_Allgather(local_solution,1,MPI_REAL8,solution,1,MPI_REAL8,comm,ierr)

  !!
  !! Check correctness of the solution
  !!
  if (procno==0) then
     do row=0,N-1
        if (abs(solution(row)-1.)>1.d-13) then
           write(*,'("Wrong solution at [",i3,"]:",f9.5)') row,solution(row)
           success = .false.
        end if
     end do
     if (success) print *,"Success"
  end if
  
  call MPI_Finalize(ierr)
  
end Program GaussJordan

