!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-5
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Persist

  use mpi
  implicit none

  integer :: src,tgt,maxsize=1000000,cnt,s,nx
  real,dimension(:),allocatable :: send,recv
  double precision,dimension(:),allocatable :: t
  integer requests(2)

#include "globalinit.F90"

#define NEXPERIMENTS 10
#define NSIZES 6

  src = 0; tgt = ntids-1
  allocate(t(NSIZES)); allocate(send(maxsize)); allocate(recv(maxsize))

  ! First ordinary communication
  s = 1
  do cnt=1,NSIZES
     if (mytid==src) then
        print *,"Size",s
        t(cnt) = MPI_Wtime()
        do nx=1,NEXPERIMENTS
           call MPI_Isend(send,s,MPI_REAL,tgt,0,comm,requests(1),err)
           call MPI_Irecv(recv,s,MPI_REAL,tgt,0,comm,requests(2),err)
           call MPI_Waitall(2,requests,MPI_STATUSES_IGNORE,err)
        end do
        t(cnt) = MPI_Wtime()-t(cnt)
     else if (mytid==tgt) then
        do nx=1,NEXPERIMENTS
           call MPI_Recv(recv,s,MPI_REAL,src,0,comm,MPI_STATUS_IGNORE,err)
           call MPI_Send(recv,s,MPI_REAL,src,0,comm,err)
        end do
     end if

     s = s*10
     if (s>maxsize) goto 10
  end do
10 continue

  if (mytid==src) then
     s = 1
     do cnt=1,NSIZES
        t(cnt) = t(cnt)/NEXPERIMENTS
        print *,"Time for pingpong of size: ",s,t(cnt)
        s = s*10
        if (s>maxsize) goto 20
     end do
  end if
20 continue

  ! Now persistent communication
  s = 1
  do cnt=1,NSIZES
    if (mytid==src) then
        print *,"Size",s
        t(cnt) = MPI_Wtime()
        call MPI_Send_init(send,s,MPI_REAL,tgt,0,comm,requests(1),err)
        call MPI_Recv_init(recv,s,MPI_REAL,tgt,0,comm,requests(2),err)
        t(cnt) = MPI_Wtime()
        do nx=1,NEXPERIMENTS
           call MPI_Startall(2,requests,err)
           call MPI_Waitall(2,requests,MPI_STATUSES_IGNORE,err)
        end do
        t(cnt) = MPI_Wtime()-t(cnt)
        call MPI_Request_free(requests(0),err)
        call MPI_Request_free(requests(1),err)
     else if (mytid==tgt) then
        do nx=1,NEXPERIMENTS
           call MPI_Recv(recv,s,MPI_REAL,src,0,comm,MPI_STATUS_IGNORE,err)
           call MPI_Send(recv,s,MPI_REAL,src,0,comm,err)
        end do
     end if
     s = s*10
     if (s>maxsize) goto 30
  end do
30 continue

  if (mytid==src) then
     s = 1
     do cnt=1,NSIZES
        t(cnt) = t(cnt)/NEXPERIMENTS
        print *,"Time for pingpong of size: ",s,t(cnt)
        s = s*10
        if (s>maxsize) goto 40
     end do
40   continue
  end if

  call MPI_Finalize(err)

end Program Persist

