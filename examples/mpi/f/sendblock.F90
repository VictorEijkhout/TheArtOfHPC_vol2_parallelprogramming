!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2013-2020
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


Program SendBlock

#include "mpif.h"

  integer :: other,size,status(MPI_STATUS_SIZE)
  integer,dimension(:),allocatable :: sendbuf,recvbuf
#include "globalinit.F90"

  if (mytid>1) goto 10
     other = 1-mytid
     size = 1
     do 
        allocate(sendbuf(size)); allocate(recvbuf(size))
        print *,size
        call MPI_Send(sendbuf,size,MPI_INTEGER,other,0,comm,err)
        call MPI_Recv(recvbuf,size,MPI_INTEGER,other,0,comm,status,err)
        if (mytid==0) then
           print *,"MPI_Send did not block for size",size
        end if
        deallocate(sendbuf); deallocate(recvbuf)
        size = size*10
        if (size>2000000000) goto 20
     end do
20   continue

10   call MPI_Finalize(err)


end program SendBlock
