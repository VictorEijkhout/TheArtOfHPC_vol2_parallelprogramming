!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2020
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Waitnull

  use mpi_f08

  implicit none

  !!
  !! General stuff
  !!
  Type(MPI_Comm) :: comm;
  integer :: mytid,ntids,i,p,err;

  !!
  !! random number generatoe
  !!
  integer :: randsize
  integer,allocatable,dimension(:) :: randseed

  !!
  !! data for this program
  !!
  Type(MPI_Request),dimension(:),allocatable :: requests
  integer,dimension(:),allocatable :: recv_buffer
  integer :: index,randomint,success = 1
  real :: randomvalue

  call MPI_Init()
  comm = MPI_COMM_WORLD
  call MPI_Comm_rank(comm,mytid)
  call MPI_Comm_size(comm,ntids)
  call MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN)

  !!
  !! seed the random number generator
  !!
  call random_seed(size=randsize)
  allocate(randseed(randsize))
  do i=1,randsize
     randseed(i) = 1023*mytid
  end do
  call random_seed(put=randseed)

  allocate(recv_buffer(ntids-1))
  allocate(requests(ntids-1))

  if (mytid==ntids-1) then
     !
     ! the last process posts a receive
     ! from every other process
     !
     do p=0,ntids-2
        call MPI_Irecv(recv_buffer(p+1),1,MPI_INTEGER,p,0,comm,&
             requests(p+1))
     end do
     !
     ! then wait to see what comes in
     !
     do p=0,ntids-2
        call MPI_Waitany(ntids-1,requests,index,MPI_STATUS_IGNORE)
        if ( .not. requests(index)==MPI_REQUEST_NULL) then
             print *,"This request should be null:",index
             success = 0
          end if
        !write(*,'("Message from",i3,":",i5)') index,recv_buffer(index)
     end do

  else
     !
     ! everyone else sends one number to the last
     ! after some random wait
     !
     call sleep(6)
     call random_number(randomvalue)
     randomint = randomvalue
     randomint = 30+mytid
     call MPI_Send(randomint,1,MPI_INTEGER, ntids-1,0,comm)
  end if

  call MPI_Allreduce(MPI_IN_PLACE,success,1,MPI_INTEGER,MPI_SUM,comm)
  if (mytid==0) then
     if (success==ntids) then
        print *,"All processes successfully concluded"
     else
        print *,"Failure on",ntids-success,"processes"
     end if
  end if

  call MPI_Finalize()

end Program Waitnull

