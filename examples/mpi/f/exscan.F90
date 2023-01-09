! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2013-2020
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


Program ExScan

  implicit none

#include "mpif.h"

  integer,dimension(:),allocatable :: recv_buffer
  integer :: status(MPI_STATUS_SIZE)
  real :: randomvalue
  integer :: randomint,sender

!codesnippet myfirst-f
  integer :: my_first=0,localsize
! localsize = .... result of local computation ...
!codesnippet end

#include "globalinit.F90"

  call random_number( randomvalue )
  localsize = 10*(randomvalue+1)

!codesnippet myfirst-f
  ! find myfirst local based on the local sizes
  call MPI_Exscan(localsize,my_first, &
       1,MPI_INTEGER,MPI_SUM,comm,err)
!codesnippet end

  write(6,'("[",i3,"] localsizes ",i3," first ",i3)') mytid,localsize,my_first

  call MPI_Finalize(err)


end program ExScan
