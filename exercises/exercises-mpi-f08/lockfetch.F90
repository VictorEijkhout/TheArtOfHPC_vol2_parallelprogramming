! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! %%%%
! %%%% This program file is part of the book and course
! %%%% "Parallel Programming for Science and Engineering"
! %%%% by Victor Eijkhout, copyright 2020-2022
! %%%%
! %%%% lockfetch.F90 : passive target synchronization & atomic update
! %%%%
! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program LockFetch
  use :: mpi_f08
  use :: iso_c_binding
  use :: posix
  implicit none

  integer :: nprocs, procno,sleep
  real :: randomfraction
  type(MPI_Comm) :: comm = MPI_COMM_WORLD

  !! data specifically for this program
  integer :: repo,supervisor
  type(MPI_Win) :: the_window
  integer :: win_buffer
  integer(kind=MPI_ADDRESS_KIND) :: winsize,zero_disp=0
  integer :: bytesofint
  integer :: zero=0, mone = -1,update,readout,assert=0, &
       workcount,ip,sum_work,my_work
  integer,dimension(:),allocatable :: nwork

  !!
  !! Start MPI
  !!
  call MPI_Init()
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  ALLOCATE( nwork(nprocs) )

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
  call MPI_Win_create(win_buffer,winsize,1,MPI_INFO_NULL,comm,the_window)

  !!
  !! The supervisor rank initializes the window
  !!
  call MPI_Win_fence(0,the_window)
  if (procno==supervisor) then
     workcount = 5 * nprocs;
     call MPI_Put( workcount,1,MPI_INTEGER, &
          repo,zero_disp,1,MPI_INTEGER,the_window)
  end if
  call MPI_Win_fence(0,the_window)

  !!
  !! Passive target epoch
  !!
  if (procno==supervisor) then
     !!
     !! Supervisor cycles until the window is zero
     !!
     do
        !!
        !! Exercise: read out the window's content using an atomic operation
        !!
        call MPI_Win_lock(MPI_LOCK_EXCLUSIVE,repo,assert,the_window)
!!!! your code here !!!!
        call MPI_Win_unlock(repo,the_window)
        print *,"Supervisor readout: ", readout
        if ( readout<=0 ) exit
     end do
     !!
     !! Finish up: gather how much work every process has done
     !! and do correctness check
     !!
     call MPI_Gather(readout,1,MPI_INT,nwork,1,MPI_INT,supervisor,comm)
     nwork(supervisor+1) = 0
     print *,"Work by proc:"
     print *,(nwork(ip),ip=1,nprocs)
     sum_work = SUM( nwork )
     if (sum_work/=workcount) then
        print *,"Found work:",sum_work,"s/b",workcount
        call MPI_Abort(comm,0)
     end if
  else 
     !!
     !! Worker processes do atomic update of the repo
     !!
     my_work = 0
     do 
        call random_number(randomfraction)
        sleep = c_usleep( int(randomfraction * 1000000) )
        call MPI_Win_lock(MPI_LOCK_EXCLUSIVE,repo,assert,the_window)
        call MPI_Fetch_and_op(mone, readout, MPI_INTEGER, repo,zero_disp, MPI_SUM, the_window)
        call MPI_Win_unlock(repo,the_window)
        if (readout<=0) exit
        my_work = my_work+1
        print *,"Proc",procno,"adding -1 to ",readout
     end do
     call MPI_Gather(my_work,1,MPI_INT,0,1,MPI_INT,supervisor,comm)
  end if

  call MPI_Win_free(the_window)

  call MPI_Finalize()

end Program LockFetch

