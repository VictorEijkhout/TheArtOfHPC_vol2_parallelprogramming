! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise to illustrate one-sided operations
!**** fortran 90/95 version
!****
!****************************************************************

Program RightPut

  use mpi
  implicit none

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors

  !! data for this exercise:
  integer :: the_window
  real*8 :: window_data
  integer :: window_elt_size
  integer(kind=MPI_ADDRESS_KIND) :: &
       window_size,displacement
  real*8 :: mydata, & !! my data that will be sent right
       leftdata=0,  & !! container for data received from left
       check
  integer :: sendto
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  mydata = procno

  !! set `sendto'
  sendto = procno+1
  if (procno==nprocs-1) sendto = MPI_PROC_NULL

  call MPI_Sizeof(window_data,window_elt_size,ierr)
  window_size = window_elt_size
  call MPI_Win_create( &
       !!solution
       window_data,window_size,window_elt_size, & ! window size, unit size
       !!solved
       MPI_INFO_NULL,comm,the_window,ierr)

  call MPI_Win_fence(0,the_window,ierr)
  displacement = 0
  call MPI_Put( &
       ! specify data to put:
       !!solution
       mydata,1,MPI_REAL8, & ! data on origin
       !!solved
       ! where are you putting it:
       ! (use an MPI_ADDRESS_KIND variable for displacement!)
       !!solution
       sendto,displacement, 1,MPI_REAL8, &
       !!solved
       the_window, ierr,ierr)
  call MPI_Win_fence(0,the_window,ierr)
  leftdata = window_data

  !!
  !! Check correctness
  !!
  if (procno.gt.0 .and. leftdata/=mydata-1) then
     error = procno
  else
     error = nprocs
  end if
  call MPI_Allreduce(error,errors,1,MPI_INTEGER,MPI_MIN,comm,ierr)
  if (procno.eq.0) then
     if (errors.eq.nprocs) then
        print *,"Finished; all results correct"
     else
        print *,"First error occurred on proc",errors
     end if
  end if

  call MPI_Win_free(the_window,ierr)
  call MPI_Finalize(ierr)
  
end Program RightPut
