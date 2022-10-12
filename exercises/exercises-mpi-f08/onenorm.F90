! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2021
!****
!**** MPI Exercise for user-defined operators
!**** fortran 2008 version
!****
!****************************************************************/

Program OneNorm

  use mpi_f08
  implicit none

  integer :: nprocs, procno,ierr
  type(MPI_Comm) :: comm

  !! data specific for this program
  Type(MPI_Op) :: norm_one_reduct
  real(4) :: actual_norm
  real(4) :: data, data_one_norm
  !real(4),pointer :: data, data_one_norm
  !allocate(data,data_one_norm)

  call MPI_Init()
  comm = MPI_COMM_WORLD
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  data = -1.e0

  call MPI_Op_create(add_abs,.true.,norm_one_reduct)
  ! https://stackoverflow.com/tags/fortran-iso-c-binding/info
  call MPI_Allreduce(data,data_one_norm,1,MPI_REAL4,norm_one_reduct,comm)
  actual_norm = nprocs
  if (data_one_norm/=actual_norm) then
     print *,procno,": result",data_one_norm," should be",actual_norm
  else if (procno==0) then
     print *,"Success"
  end if

  call MPI_Finalize()
  
contains
  subroutine add_abs(invec,inoutvec,len,datatype) ! error? bind(c)
    use, intrinsic :: iso_c_binding, only : c_ptr,c_f_pointer
    use mpi_f08

    implicit none

    ! parameters
    type(c_ptr), value :: invec,inoutvec
    Integer :: len
    Type(MPI_Datatype) :: datatype

    ! local variables
    Real(4),pointer :: invec_r(:),inoutvec_r(:)

    ! sanity check
    if (datatype%MPI_VAL/=MPI_REAL4%MPI_VAL) then
       stop "This is not an MPI_REAL4"
    end if

    ! extract c_ptr to f pointer
    call c_f_pointer( invec,invec_r, (/len/) )
    call c_f_pointer( inoutvec,inoutvec_r, (/len/) )
    !n = invec; r = inoutvec
    !!
    !! Exercise:
    !! -- do the reduction step for a 1-norm
    !!
!!!! your code here !!!!

  end subroutine add_abs
end Program OneNorm
