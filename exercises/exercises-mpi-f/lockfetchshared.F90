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
  use mpi
  implicit none

  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  !! data specifically for this program
  integer :: repo,supervisor
  integer :: the_window
  integer :: win_buffer
  integer(kind=MPI_ADDRESS_KIND) :: winsize,zero_disp=0
  integer :: bytesofint
  integer :: zero=0, one = 1,update,readout,assert=0

  !!
  !! Start MPI
  !!
  call MPI_Init(ierr)
  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Setup window and data:
  !! the window is only nonzero on procno==repo; then it is 1 integer
  !!
  repo = 0; supervisor = nprocs-1
  call MPI_Sizeof(nprocs,bytesofint,ierr)
  if (procno==repo) then
     winsize = bytesofint
  else
     winsize = 0
  end if
  call MPI_Win_create(win_buffer,winsize,1,MPI_INFO_NULL,comm,the_window,ierr)

  !!
  !! The supervisor rank initializes the window
  !!
  call MPI_Win_fence(0,the_window,ierr)
  if (procno==supervisor) then
     call MPI_Put( zero,1,MPI_INTEGER, &
          repo,zero_disp,1,MPI_INTEGER,the_window,ierr)
  end if
  call MPI_Win_fence(0,the_window,ierr)

  !!
  !! Passive target epoch
  !!
  call MPI_Win_lock(MPI_LOCK_SHARED,repo,assert,the_window,ierr)
  if (procno==supervisor) then
     do
        !!
        !! Exercise: read out the window's content using an atomic operation
        !!

!!!! your code here !!!!
        print *,"Supervisor readout: ", readout
        if ( readout>=nprocs-1 ) exit
     end do
     print *,"Supervisor is done!"
  else 
     !!
     !! Atomic update of the repo
     !!
     call MPI_Fetch_and_op(one, readout, MPI_INTEGER, repo,zero_disp, MPI_SUM, the_window,ierr)
     call MPI_Win_flush_local(repo,the_window,ierr)
     print *,"Proc",procno,"adding 1 to ",readout
  end if
  ! if (procno==supervisor) &
  !   print *,"Supervisor window readout=",readout
  ! if (procno==repo) &
  !   print *,"Window buffer=",win_buffer
  call MPI_Win_unlock(repo,the_window,ierr)
  call MPI_Win_free(the_window,ierr)

  call MPI_Finalize(ierr)

end Program LockFetch

