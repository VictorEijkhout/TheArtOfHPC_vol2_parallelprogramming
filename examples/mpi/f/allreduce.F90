!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Allreduce

  use mpi
  implicit none

  real :: mynumber,result

#include "globalinit.F90"

  mynumber = mytid
  ! add the random variables together
  call MPI_Allreduce(mynumber,result,&
  		1,MPI_REAL,MPI_SUM,comm,err)
  ! the result should be ntids(ntids-1)/2:
  if (mytid.eq.ntids-1) then
    write(*,'("Result ",f5.2," compared to ntids(ntids-1)/2=",f5.2)') &
         result,ntids*(ntids-1)/2.
 end if

  call MPI_Finalize(err)

end Program Allreduce

