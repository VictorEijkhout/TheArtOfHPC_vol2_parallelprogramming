!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2013-5
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


Program SsendBlock

#include "mpif.h"

  integer :: other,size,status(MPI_STATUS_SIZE)
  integer,dimension(:),allocatable :: sendbuf,recvbuf
#include "globalinit.F90"

  if (mytid>1) goto 10
  other = 1-mytid
  size = 1
  allocate(sendbuf(size)); allocate(recvbuf(size))
  call MPI_Ssend(sendbuf,size,MPI_INTEGER,other,0,comm,err)
  call MPI_Recv(recvbuf,size,MPI_INTEGER,other,0,comm,status,err)
  print *,"This statement is not reached"

10   call MPI_Finalize(err)

end program SsendBlock
