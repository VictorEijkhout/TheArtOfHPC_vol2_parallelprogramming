! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-5
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Pack

  use mpi
  implicit none

  integer :: sender=0,receiver,&
       nsends=50,maxsends=100

  integer :: buflen = 1000,position,len,irecv_value
  character,dimension(:),allocatable :: buffer
  double precision :: val, xrecv_value

#include "globalinit.F90"

  if (ntids<2) then
    print *,"This program needs at least two processes"
    stop
  end if

  receiver=ntids-1
  ALLOCATE(buffer(buflen))

  ! if (argc<2) {
  !   printf("Usage: %s nnn\n",argv[0]);
  !   return -2;
  ! }

  !  int nsends=atoi(argv[1]),maxsends=100;

  if (nsends>maxsends) then
     print *,"Max value for commandline argument:",maxsends
     stop
  end if

  if (mytid==sender) then
     len = 2*4+nsends*8 ! 2*sizeof(int)+nsends*sizeof(double)
     if (len>buflen) then
        print *,"Not enough buffer space, need",len
        stop
     end if
     position = 0
     call MPI_Pack(nsends,1,MPI_INTEGER,buffer,buflen,position,comm,err)
     do i=1,nsends
        val = 1.35 ! rand()/(double)RAND_MAX;
        call MPI_Pack(val,1,MPI_DOUBLE_PRECISION,buffer,buflen,position,comm,err)
     end do
     call MPI_Pack(nsends,1,MPI_INTEGER,buffer,buflen,position,comm,err)
     call MPI_Send(buffer,position,MPI_PACKED,receiver,0,comm,err)
  else if (mytid==receiver) then
     call MPI_Recv(buffer,buflen,MPI_PACKED,sender,0,comm,MPI_STATUS_IGNORE,err)
     position = 0
     call MPI_Unpack(buffer,buflen,position,nsends,1,MPI_INT,comm,err)
     do i=1,nsends
        call MPI_Unpack(buffer,buflen,position,xrecv_value,1,MPI_DOUBLE_PRECISION,comm,err)
     end do
     call MPI_Unpack(buffer,buflen,position,irecv_value,1,MPI_INTEGER,comm,err)
     if (.not. irecv_value==nsends) &
        print *,"found",irecv_value,"should have been",nsends
  end if

  call MPI_Finalize(err)

end program Pack
