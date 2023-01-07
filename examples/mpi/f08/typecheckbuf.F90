! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%% typecheckbuf.F90 : buffer types are not checked
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Typecheck

  use mpi_f08
  implicit none

  !!codesnippet ftypecheckbuf
  integer,parameter :: n=1
  Real,dimension(n) :: source
  call MPI_Init()
  call MPI_Send(source,n,MPI_INTEGER, &
       1,0,MPI_COMM_WORLD)
  !!codesnippet end

  call MPI_Finalize()

end Program Typecheck
