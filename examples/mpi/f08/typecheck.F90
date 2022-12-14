! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%% typecheck.F90 : compiler can not find type mismatches type/buffer
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Typecheck

  use mpi_f08
  implicit none

  !!codesnippet ftypecheckbuf
  Real :: source
  integer :: n=1
  call MPI_Init()
  call MPI_Send(source,MPI_INTEGER,n, &
       1,0,MPI_COMM_WORLD)
  !!codesnippet end

  call MPI_Finalize()

end Program Typecheck
