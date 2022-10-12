! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for random one-sided operations
!**** fortran 2008 version
!****
!****************************************************************

Program RandomPut

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr

  type(MPI_Win) :: the_window
  integer :: window_data,window_elt_size
  integer(kind=MPI_ADDRESS_KIND) :: window_size,sizeofint,displacement
  integer :: err,iteration
  real randomfraction

  integer :: my_number=1, my_sum = 0, other

  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  !!
  !! Exercise 1:
  !! -- Take the variable `window_data' and make it into a window
  !!    of size 1 integer
  !! Exercise 2:
  !! -- Replace MPI_Win_create by MPI_Win_allocate
  !!
  call MPI_Sizeof(window_data,window_elt_size,err)
  window_size = window_elt_size
  call MPI_Win_create( &
!!!! your code here !!!!
       MPI_INFO_NULL,comm,the_window,err)

  !!
  !! Put twenty numbers from proc 0 into 1 or 2,
  !! depending on random chance
  !!
  do iteration=1,20
     !!
     !! compute `other': where to put the data
     call random_number(randomfraction)
     if (randomfraction<.5) then
        other = 1
     else
        other = 2
     end if

     !!
     !! receiving procs set their window to zero
     !!
     if (procno==1 .or. procno==2) window_data = 0

     !!
     !! Now we have an epoch where proc 0 puts data either in 1 or 2
     !!
     call MPI_Win_fence(0,the_window,err)
     if (procno==0) then
        displacement = 0
        call MPI_Put( &
             ! specify data to put:
!!!! your code here !!!!
             ! where are you putting it:
             ! (use an MPI_ADDRESS_KIND variable for displacement!)
!!!! your code here !!!!
             the_window, err)
     end if
     call MPI_Win_fence(0,the_window,err)
     my_sum = my_sum+window_data

  end do
  
  !!
  !! Now check that data received on 1 & 2 together adds up to 20
  !!
  if (procno==1 .or. procno==2) then
     print *,"Sum on",procno,my_sum
  else if (procno==0) then
     print *,"Sums on 1 and 2 together should be: 20"
  end if
  
  call MPI_Win_free(the_window,err)
  
  call MPI_Finalize()
  
end Program RandomPut

