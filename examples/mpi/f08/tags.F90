! /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%
!    %%%% This program file is part of the book and course
!    %%%% "Parallel Computing"
!    %%%% by Victor Eijkhout, copyright 2013-2022
!    %%%%
!    %%%% tags.F90 : getting tgs upper bound
!    %%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! */

Program Tags
  
  use mpi_f08
  implicit none
  
#ifndef FREQUENCY
#define FREQUENCY -1
#endif

  type(MPI_Comm) :: comm;
  integer :: nprocs,procno,ierr

  !!
  !! local data
  !!
  integer :: sender,receiver
  !!codesnippet gettagubf
  logical :: flag
  integer(KIND=MPI_ADDRESS_KIND) :: attr_v,tag_upperbound
  !!codesnippet end
  
  !!
  !! Initial setup
  !!
  call MPI_Init()
  comm = MPI_COMM_WORLD;
  call MPI_Comm_rank(comm,procno)
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN)

  if (nprocs<2) then 
     print *,"This program needs at least two processes"
     call MPI_Abort(comm,1)
  end if

  sender = 0; receiver = nprocs-1
  if (procno==0) then
     print '("Running on comm world of",i4," procs; ", &
          "communicating between ",i4,"--",i4)', &
          nprocs,sender,receiver
  end if

  !!codesnippet gettagubf
  call MPI_Comm_get_attr(comm,MPI_TAG_UB,attr_v,flag,ierr)
  tag_upperbound = attr_v
  !!codesnippet end

  if (ierr/=MPI_SUCCESS) then 
     print '("Error getting attribute: return code=",i4)',ierr
     if (ierr==MPI_ERR_COMM) &
          print '("invalid communicator")'
     if (ierr==MPI_ERR_KEYVAL) &
          print '("erroneous keyval\n")'
     call MPI_Abort(comm,0)
  end if
  if ( .not. flag) then
     print '("Could not get keyval")'
     call MPI_Abort(comm,0)
  else 
     if (procno==sender) then
        !!codesnippet gettagubf
        print '("Determined tag upperbound: ",i9)', tag_upperbound
        !!codesnippet end
     end if
  end if
  
  call MPI_Finalize();

end Program Tags
