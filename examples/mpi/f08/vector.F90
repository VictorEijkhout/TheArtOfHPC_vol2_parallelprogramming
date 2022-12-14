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

Program Vector

  use mpi_f08
  implicit none

  double precision, dimension(:),allocatable :: source,target
  integer :: sender = 0,receiver = 1, count = 5, stride = 2

  !!codesnippet vector-f08-type
  Type(MPI_Datatype) :: newvectortype
  !!codesnippet end
  integer :: recv_count
  Type(MPI_Status) :: recv_status

  Type(MPI_Comm) :: comm;
  integer :: mytid,ntids,i,p,err;

  call MPI_Init()
  comm = MPI_COMM_WORLD
  call MPI_Comm_rank(comm,mytid)
  call MPI_Comm_size(comm,ntids)
  call MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN)


  if (ntids<2) then
    print *,"This program needs at least two processes"
    stop
  end if

  ALLOCATE(source(stride*count))
  ALLOCATE(target(stride*count))

  do i=1,stride*count
     source(i) = i+.5;
  end do

  !!codesnippet vector-f08
  if (mytid==sender) then
     call MPI_Type_vector(count,1,stride,MPI_DOUBLE_PRECISION,&
          newvectortype)
     call MPI_Type_commit(newvectortype)
     call MPI_Send(source,1,newvectortype,receiver,0,comm)
     call MPI_Type_free(newvectortype)
     if ( .not. newvectortype==MPI_DATATYPE_NULL) then
        print *,"Trouble freeing datatype"
     else
        print *,"Datatype successfully freed"
     end if
  else if (mytid==receiver) then
     call MPI_Recv(target,count,MPI_DOUBLE_PRECISION,sender,0,comm,&
          recv_status)
     call MPI_Get_count(recv_status,MPI_DOUBLE_PRECISION,recv_count)
  end if
  !!codesnippet end
  
  if (mytid==receiver) then
    ! for (i=0; i<count; i++)
    !   if (target[i]!=source[stride*i])
    !     printf("location %d %e s/b %e\n",i,target[i],source[stride*i]);
  end if

  call MPI_Finalize(err)

end Program Vector
