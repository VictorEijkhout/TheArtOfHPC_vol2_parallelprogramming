!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Irecv_source

  use mpi
  implicit none

  integer,dimension(:),allocatable :: recv_buffer,requests
  integer :: index,randomint
  real :: randomvalue
#include "globalinit.F90"
  allocate(recv_buffer(ntids-1))
  allocate(requests(ntids-1))

  if (mytid==ntids-1) then
     do p=1,ntids-1
        print *,"post"
        call MPI_Irecv(recv_buffer(p),1,MPI_INTEGER,p-1,0,comm,&
             requests(p),err)
     end do
     do p=1,ntids-1
        call MPI_Waitany(ntids-1,requests,index,MPI_STATUS_IGNORE,err)
        write(*,'("Message from",i3,":",i5)') index,recv_buffer(index)
     end do
  else
     call sleep(6)
     call random_number(randomvalue)
     randomint = randomvalue
     randomint = 30+mytid
     call MPI_Send(randomint,1,MPI_INTEGER, ntids-1,0,comm,err)
  end if

end Program Irecv_source

