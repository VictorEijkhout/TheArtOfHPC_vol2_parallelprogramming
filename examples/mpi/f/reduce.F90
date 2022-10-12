!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Reduce

  use mpi
  implicit none

  real :: mynumber,result
  integer :: target_proc

#include "globalinit.F90"

  mynumber = mytid
  target_proc = ntids-1;
  ! add all the random variables together
  call MPI_Reduce(mynumber,result,1,MPI_REAL,MPI_SUM,&
             target_proc,comm,err)
  ! the result should be approx ntids/2:
  if (mytid.eq.target_proc) then
    write(*,'("Result ",f5.2," compared to ntids(ntids-1)/2=",f5.2)') &
         result,ntids*(ntids-1)/2.
 end if

  call MPI_Finalize(err)

end Program Reduce

