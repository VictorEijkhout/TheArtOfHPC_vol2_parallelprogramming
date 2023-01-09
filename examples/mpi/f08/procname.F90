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

Program ProcName
  use mpi_f08
  implicit none

  Type(MPI_Comm) :: comm
  integer :: nprocs,procid,target,sendcount,recvcount
  !!codesnippet procnamef
  Character(len=MPI_MAX_PROCESSOR_NAME) :: proc_name
  Integer :: len
  !!codesnippet end
  
  call MPI_Init()
  comm = MPI_COMM_WORLD
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procid)

  !!codesnippet procnamef
  len = MPI_MAX_PROCESSOR_NAME
  call MPI_Get_processor_name(proc_name,len)
  print *,"Proc",procid,"runs on ",proc_name(1:len),"."
  !!codesnippet end

  call MPI_Finalize()

end Program ProcName
