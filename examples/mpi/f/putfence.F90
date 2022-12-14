! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-5
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program PutFence

  use mpi
  implicit none

!!codesnippet putblock-f
  integer :: the_window,window_elt_size
  integer(kind=MPI_ADDRESS_KIND) :: window_size,target_displacement
!!codesnippet end
  integer :: my_number=0, window_data(2), other

#include "globalinit.F90"
  other = ntids-1
  if (mytid.eq.0) my_number = 37
  target_displacement = 1

!!codesnippet putblock-f
  call MPI_Sizeof(window_data,window_elt_size,err)
  window_size = 2*window_elt_size
  call MPI_Win_create(window_data,&
       window_size,window_elt_size,          & ! window size, unit size
       MPI_INFO_NULL,comm,the_window,err)
  call MPI_Win_fence(0,the_window,err)
  if (mytid.eq.0) then
     call MPI_Put( my_number, 1,MPI_INTEGER, & ! data
          other,target_displacement,         & ! location
          1,MPI_INTEGER, &
          the_window,err)
 endif
 call MPI_Win_fence(0,the_window,err)
!!codesnippet end
 if (mytid.eq.other) then
      print *,"I got:",window_data(1+target_displacement)
   end if
!!codesnippet putblock-f
   call MPI_Win_free(the_window,err)
!!codesnippet end

   call MPI_Finalize(err);

 end Program PutFence

