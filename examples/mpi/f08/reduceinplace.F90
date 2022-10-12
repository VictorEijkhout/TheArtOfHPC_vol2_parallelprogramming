!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2021
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program ReduceInPlace

  use mpi_f08

  real :: mynumber,result
  integer :: target_proc

#include "globalinit.F90"

  call random_number(mynumber)
  target_proc = ntids-1;
  ! add all the random variables together
  if (mytid.eq.target_proc) then
     result = mytid
     call MPI_Reduce(MPI_IN_PLACE,result,1,MPI_REAL,MPI_SUM,&
          target_proc,comm)
  else
     mynumber = mytid
     call MPI_Reduce(mynumber,result,1,MPI_REAL,MPI_SUM,&
          target_proc,comm)
  end if
  ! the result should be ntids*(ntids-1)/2:
  if (mytid.eq.target_proc) then
    write(*,'("Result ",f5.2," compared to n(n-1)/2=",f5.2)') &
         result,ntids*(ntids-1)/2.
 end if

  call MPI_Finalize(err)

end Program ReduceInPlace
