!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Gather

  use mpi
  implicit none

  integer :: localsize,root
  integer,allocatable,dimension(:) :: localsizes

#include "globalinit.F90"

  localsize = 10+10*random_value
  root = ntids-1

  ! we assume that each process has a value "localsize"
  ! the root process collectes these values
  if (mytid==root) then
     ALLOCATE(localsizes(ntids))
  end if

  call MPI_Gather(localsize,1,MPI_INTEGER,&
       localsizes,1,MPI_INTEGER,root,comm,err)
  if (mytid==root) then
     write(*,'("Local sizes:",10i4)') (localsizes(i),i=1,ntids)
  end if

  call MPI_Finalize(err)

end Program Gather
