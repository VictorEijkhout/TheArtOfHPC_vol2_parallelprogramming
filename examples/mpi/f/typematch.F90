! /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%
!    %%%% This program file is part of the book and course
!    %%%% "Parallel Computing"
!    %%%% by Victor Eijkhout, copyright 2013-6
!    %%%%
!    %%%% typematch.F90
!    %%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! */

Program typematch
  use mpi
  implicit none

  integer, parameter :: &
       r5=selected_real_kind(5), &
       r10=selected_real_kind(10)
  real(kind=r5) :: x5
  real(kind=r10) :: x10
  integer :: s5,s10,ierr
  
  !  type(MPI_Datatype) :: mpi_x5,mpi_x10
  integer :: mpi_x5,mpi_x10

  call MPI_Init(ierr)

  call MPI_Type_match_size(MPI_TYPECLASS_REAL,4,mpi_x5,ierr)
  call MPI_Type_match_size(MPI_TYPECLASS_REAL,8,mpi_x10,ierr)
  call MPI_Type_size(mpi_x5,s5,ierr)
  call MPI_Type_size(mpi_x10,s10,ierr)
  print *,s5,s10

  call MPI_Finalize(ierr)

end Program typematch
