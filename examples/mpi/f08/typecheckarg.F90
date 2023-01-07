! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%% typecheckarg.F90 : argument order errors are found
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Typecheck

  use mpi_f08
  implicit none

  !!codesnippet ftypecheckarg
  integer,parameter :: n=2
  Integer,dimension(n) :: source
  call MPI_Init()
  call MPI_Send(source,MPI_INTEGER,n, &
       1,0,MPI_COMM_WORLD)
  !!codesnippet end

  call MPI_Finalize()

end Program Typecheck
