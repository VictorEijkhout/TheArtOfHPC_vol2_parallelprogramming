! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise
!**** onesidedbuild.F90 : put random data everywhere
!**** fortran 2008 version
!****
!****************************************************************/

Program OnesidedBuild

  use mpi_f08
  implicit none

  integer :: nprocs, procno,ierr
  type(MPI_Comm) :: comm = MPI_COMM_WORLD

  ! window stuff
  type(MPI_Win) :: the_window
  integer :: window_elt_size
  integer(kind=MPI_ADDRESS_KIND) :: window_size,sizeofint,displacement=0
#define MAXFULL 100
  integer :: mytable(MAXFULL);

  ! random stuff
  integer,allocatable,dimension(:) :: randseed
  integer :: randsize
  real(4) :: randomfraction

  ! other program variables
#define NUMBERS 2
  integer :: targets(NUMBERS)
  integer :: i,inum,other_process
  integer :: increment_by_1=1,stack_pointer,number_for_other
  integer(kind=MPI_ADDRESS_KIND) :: pointer_loc,offset

  call MPI_Init()
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

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
  !! Make a window exposing my table
  !!
  mytable(1) = 0; ! set stack pointer to first open location
  call MPI_Sizeof(mytable(1),window_elt_size)
  window_size = MAXFULL*window_elt_size
  call MPI_Win_create( &
       mytable,window_size,window_elt_size, &
       MPI_INFO_NULL,comm,the_window)

  do inum=1,NUMBERS
     call random_number(randomfraction)
     other_process = nprocs * randomfraction
     targets(inum) = other_process

     !!
     !! - Lock the window on the other process
     !! - Get first open location on the target and increment this
     !! - Write the random number
     !!
     call MPI_Win_lock(MPI_LOCK_EXCLUSIVE,other_process,0,the_window)

     pointer_loc = 0
     !!
     !! Exercise:
     !! - add 1 to the stack pointer on the other process
     !! - which routine do you use? the window is locked so maybe put/get?
     !!
!!!! your code here !!!!

     !!
     !! Exercise: write the number
     !! - `stackpointer' now describes where you can safely write in the other process
     !! - what call are you using to transfer data? are there possible conflicts?
     !! - is further synchronization needed?
     !!
     number_for_other = procno
     offset = stack_pointer+1
!!!! your code here !!!!

     !! Unlock the window so that other processes can access it
     call MPI_Win_unlock(other_process,the_window)
  end do
  
  call MPI_Win_free(the_window)

  if (nprocs>99) print *,"Warning: no space for procno in format"
  if (NUMBERS>50) print *,"Warning: no space for targets in format"
  print '("[",i2"] wrote its id to:",50i2)',procno,(targets(i),i=1,NUMBERS)
  print '("[",i2"] received:",50i2)',procno,(mytable(i),i=1,mytable(1))
  
  call MPI_Finalize()
  
end Program OnesidedBuild

