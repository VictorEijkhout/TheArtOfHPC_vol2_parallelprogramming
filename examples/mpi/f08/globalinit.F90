! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2013-5
!  %%%%
!  %%%% globalinit.F90 : intro for any MPI program, F08 version
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

!!codesnippet chkerr-f
#define CHK(x) 
!#define CHK(x) if (x) {				         \
!     char errtxt[200]; int len=200;				 \
!   MPI_Error_string(x,errtxt,&len);				 \
!   printf("p=%d, line=%d, err=%d, %s\n",mytid,__LINE__,x,errtxt); \
!   return x;}
!!codesnippet end

  Type(MPI_Comm) :: comm;
  integer :: mytid,ntids,i,p,err;
! stuff for the random number generator
  integer :: randsize
  integer,allocatable,dimension(:) :: randseed
  real :: random_value

  call MPI_Init(err); CHK(err);
  comm = MPI_COMM_WORLD;
  call MPI_Comm_rank(comm,mytid)
  call MPI_Comm_size(comm,ntids)
  call MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN)

  call random_seed(size=randsize)
  allocate(randseed(randsize))
  do i=1,randsize
     randseed(i) = 1023*mytid
  end do
  call random_seed(put=randseed)

!  call random_seed()
!  call random_number(random_value)

!  // Initialize the random number generator
!  srand((int)(mytid*(double)RAND_MAX/ntids));
