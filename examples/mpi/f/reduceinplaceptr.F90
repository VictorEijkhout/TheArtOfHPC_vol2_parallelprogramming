!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program ReduceInPlace

  use mpi
  implicit none

  integer :: target_proc
  real,target :: mynumber,result,in_place_val
  real,pointer :: mynumber_ptr,result_ptr

#include "globalinit.F90"

  call random_number(mynumber)
  target_proc = ntids-1;
  in_place_val = MPI_IN_PLACE
  if (mytid.eq.target_proc) then
     result_ptr => result
     mynumber_ptr => in_place_val
     result_ptr = mytid
  else
     mynumber_ptr => mynumber
     result_ptr = in_place_val
     mynumber_ptr = mytid
  end if
  call MPI_Reduce(mynumber_ptr,result_ptr,1,MPI_REAL,MPI_SUM,&
       target_proc,comm,err)

  ! the result should be ntids*(ntids-1)/2:
  if (mytid.eq.target_proc) then
    write(*,'("Result ",f5.2," compared to n(n-1)/2=",f5.2)') &
         result,ntids*(ntids-1)/2.
 end if

  call MPI_Finalize(err)

end Program ReduceInPlace
