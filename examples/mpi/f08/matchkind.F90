! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-19
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program matchkind
  use mpi_f08
  implicit none

  integer,parameter :: &
       r05=selected_real_kind(5), &
       r10=selected_real_kind(10), &
       r15=selected_real_kind(15), &
       r20=selected_real_kind(20)
  real(kind=r05) :: x05
  real(kind=r10) :: x10
  real(kind=r15) :: x15
  real(kind=r20) :: x20
  integer :: s05,s10,s15,s20, ierr
  
  type(MPI_Datatype) :: &
       mpi_x05,mpi_x10,mpi_x15,mpi_x20

  call MPI_Init()

  if (r05>0) then
     call MPI_Sizeof(x05,s05,ierr)
     call MPI_Type_match_size(MPI_TYPECLASS_REAL,s05,mpi_x05)
     call MPI_Type_size(mpi_x05,s05)
     print *,"05 positions supported, MPI type size is",s05
  end if
  if (r10>0) then
     call MPI_Sizeof(x10,s10,ierr)
     call MPI_Type_match_size(MPI_TYPECLASS_REAL,s10,mpi_x10)
     call MPI_Type_size(mpi_x10,s10)
     print *,"10 positions supported, MPI type size is",s10
  end if
  if (r15>0) then
     call MPI_Sizeof(x15,s15,ierr)
     call MPI_Type_match_size(MPI_TYPECLASS_REAL,s15,mpi_x15)
     call MPI_Type_size(mpi_x15,s15)
     print *,"15 positions supported, MPI type size is",s15
  end if
  if (r20>0) then
     call MPI_Sizeof(x20,s20,ierr)
     call MPI_Type_match_size(MPI_TYPECLASS_REAL,s20,mpi_x20)
     call MPI_Type_size(mpi_x20,s20)
     print *,"20 positions supported, MPI type size is",s20
  end if

  call MPI_Finalize()

end Program matchkind
