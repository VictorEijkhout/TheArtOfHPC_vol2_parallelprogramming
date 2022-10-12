! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2020-2022
!  %%%% 
!  %%%% anysource.F90 : source wildcarding, f08 version
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


Program AnySource

  use mpi_f08

  implicit none

  integer,dimension(:),allocatable :: recv_buffer
  Type(MPI_Status)  :: status
  real :: randomvalue
  integer :: randomint,sender

#include "globalinit.F90"

      if (mytid.eq.ntids-1) then
         allocate(recv_buffer(ntids-1))
         do p=0,ntids-2
            call MPI_Recv(recv_buffer(p+1),1,MPI_INTEGER,&
                 MPI_ANY_SOURCE,0,comm,status)
            sender = status%MPI_SOURCE
            print *,"Message from",sender
         end do
      else
         call random_number(randomvalue)
         randomint = randomvalue*ntids
         call sleep(randomint)
         print *,mytid,"waits for",randomint
         call MPI_Send(p,1,MPI_INTEGER,ntids-1,0,comm)
      end if

  call MPI_Finalize(err)


end program AnySource
