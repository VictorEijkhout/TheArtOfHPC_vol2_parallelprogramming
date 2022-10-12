! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2020-2022
!  %%%% 
!  %%%% count.F90 : get count from status
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program GetCount
  use mpi_f08
  implicit none

  Type(MPI_Comm) :: comm
  integer :: nprocs,procid,target,sendcount,recvcount
  integer,parameter :: N=10
  real :: buffer(N),fraction
  Type(MPI_Status) :: status
  
  call MPI_Init()
  comm = MPI_COMM_WORLD
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procid)
  target = nprocs-1

  if (procid==0) then
     sendcount = N
     call random_number(fraction)
     if (fraction>.5) then
        print *,"One less" ; sendcount = N-1
     end if
     call MPI_Send( buffer,sendcount,MPI_REAL,target,0,comm )
  else if (procid==target) then
    call MPI_Recv( buffer,N,MPI_REAL,0,0, comm, status )
    call MPI_Get_count(status,MPI_FLOAT,recvcount)
    print *,"Received",recvcount,"elements"
 end if

  call MPI_Finalize()

end Program GetCount
