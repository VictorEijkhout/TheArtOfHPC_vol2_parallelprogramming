! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2020-2023
!  %%%% 
!  %%%% kindsend.F90 : reductino on kind-defined type
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


Program KindSend
  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: rank, ntask, ierr, stat

  !!codesnippet kindsend
  Integer,parameter :: digits=16 
  Integer,parameter :: ip = Selected_Int_Kind(digits)
  Integer (kind=ip) :: data
  Type(MPI_Datatype) :: mpi_ip
  !!codesnippet end
  Integer :: i

  call MPI_init()

  call MPI_Comm_size(comm, ntask)
  call MPI_Comm_rank(comm, rank)

  !!codesnippet kindsend
  Call MPI_Type_create_f90_integer(digits,mpi_ip)
  !!codesnippet end

  !!codesnippet kindsend
  if (rank==0) then
     print *,"Fortran type has range",range(data)
  !!codesnippet end
     if (range(data)<digits) &
          print *,"DANGER could not accomodate #digits=",digits
     data = 1
     do i=1,3
        data = data * 90000
     end do
     if (data<0) print *,"OVERFLOW"
     print '(a10,i17)',"Sending:",data
  !!codesnippet kindsend
     call MPI_Send( data,1,mpi_ip, 1,0,comm )
  else if (rank==1) then
     call MPI_Recv( data,1,mpi_ip, 0,0,comm, MPI_STATUS_IGNORE )
  !!codesnippet end
     print '(a10,i17)',"Received:",data
  end if

  call MPI_Finalize()

End Program KindSend
