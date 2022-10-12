! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2020
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program PutFence

  use mpi_f08
  implicit none

  Type(MPI_Win) :: the_window
  integer :: window_elt_size
  integer(kind=MPI_ADDRESS_KIND) :: window_size
  integer(kind=MPI_ADDRESS_KIND) :: target_displacement
  integer :: my_number=0, window_data(2), other

  Type(MPI_Comm) :: comm;
  integer :: mytid,ntids,i,p,err;

  call MPI_Init()
  comm = MPI_COMM_WORLD
  call MPI_Comm_rank(comm,mytid)
  call MPI_Comm_size(comm,ntids)
  call MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN)

  other = ntids-1
  if (mytid.eq.0) my_number = 37

  call MPI_Sizeof(window_data,window_elt_size)
  window_size = 2*window_elt_size
  call MPI_Win_create(window_data,&
       window_size,window_elt_size,          & ! window size, unit size
       MPI_INFO_NULL,comm,the_window)
  call MPI_Win_fence(0,the_window)
  if (mytid.eq.0) then
#ifndef F90STYLE
     target_displacement = 1
     call MPI_Put( my_number, 1,MPI_INTEGER, &
          other,target_displacement, &
          1,MPI_INTEGER, &
          the_window)
#else
     call MPI_Put( my_number, 1,MPI_INTEGER, &
          other,1, &
          1,MPI_INTEGER, &
          the_window)
#endif
 endif
 call MPI_Win_fence(0,the_window)

 if (mytid.eq.other) then
      print *,"I got:",window_data(1+target_displacement)
   end if
   call MPI_Win_free(the_window)


   call MPI_Finalize(err);

 end Program PutFence

