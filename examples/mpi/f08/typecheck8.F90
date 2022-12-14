! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Typecheck

  use mpi_f08
  implicit none

  !!codesnippet ftypecheck8
  integer(8) :: source,n=1
  call MPI_Init()
  call MPI_Send(source,n,MPI_INTEGER8, &
       1,0,MPI_COMM_WORLD)
  !!codesnippet end

  call MPI_Finalize()

end Program Typecheck
