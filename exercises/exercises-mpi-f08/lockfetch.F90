! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! %%%%
! %%%% This program file is part of the book and course
! %%%% "Parallel Programming for Science and Engineering"
! %%%% by Victor Eijkhout, copyright 2020
! %%%%
! %%%% lockfetch.c : passive target synchronization & atomic update
! %%%%
! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program LockFetch
  use mpi_f08
  use iso_c_binding
  implicit none

  integer :: nprocs, procno,ierr
  type(MPI_Comm) :: comm = MPI_COMM_WORLD

  !! data specifically for this program
  integer :: repo,supervisor
  type(MPI_Win) :: the_window
  integer :: win_buffer
  integer(kind=MPI_ADDRESS_KIND) :: winsize,zero_disp=0
  integer :: bytesofint
  integer :: zero=0, one = 1,update,readout,assert=0

  !!
  !! Start MPI
  !!
  call MPI_Init()
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  !!
  !! Setup window and data:
  !! the window is only nonzero on procno==repo; then it is 1 integer
  !!
  repo = 0; supervisor = nprocs-1
  !! deprecated call MPI_Sizeof(nprocs,bytesofint)
  bytesofint = c_sizeof(nprocs)
  if (procno==repo) then
     winsize = bytesofint
  else
     winsize = 0
  end if
  !call MPI_Win_allocate( winsize, bytesofint, comm, win_buffer, the_window)
  call MPI_Win_create(win_buffer,winsize,1,MPI_INFO_NULL,comm,the_window)

  !!
  !! The supervisor rank initializes the window
  !!
  call MPI_Win_fence(0,the_window)
  if (procno==supervisor) then
     call MPI_Put( zero,1,MPI_INTEGER, &
          repo,zero_disp,1,MPI_INTEGER,the_window)
  end if
  call MPI_Win_fence(0,the_window)

  !!
  !! Passive target epoch
  !!
  if (procno==supervisor) then
     do
        !!
        !! Exercise: read out the window's content using an atomic operation
        !!
        call MPI_Win_lock(MPI_LOCK_EXCLUSIVE,repo,assert,the_window)
!!!! your code here !!!!
        call MPI_Win_unlock(repo,the_window)
        print *,"Supervisor readout: ", readout
        if ( readout>=nprocs-1 ) exit
     end do
     print *,"Supervisor is done!"
  else 
     !!
     !! Atomic update of the repo
     !!
     call MPI_Win_lock(MPI_LOCK_EXCLUSIVE,repo,assert,the_window)
     call MPI_Fetch_and_op(one, readout, MPI_INTEGER, repo,zero_disp, MPI_SUM, the_window)
     call MPI_Win_unlock(repo,the_window)
     print *,"Proc",procno,"adding 1 to ",readout
  end if
  if (procno==supervisor) &
    print *,"Supervisor final readout",readout

  call MPI_Barrier(comm)
  call MPI_Win_free(the_window)

  call MPI_Finalize()

end Program LockFetch

