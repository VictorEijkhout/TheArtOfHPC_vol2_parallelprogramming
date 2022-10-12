!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


Program SendRecv

  implicit none
#include "mpif.h"

  integer :: other,size,status(MPI_STATUS_SIZE)
  integer :: right,left, my_data, other_data, reference_data
#include "globalinit.F90"

  if (mytid>2) goto 10
  if (ntids<3) then
    print *,"This program needs at least 3 processes"
    goto 10
 end if

! We use processors 0,1,2 to set up a ring

  right = mod(mytid+1,3); left = mod(mytid+2,3);
  my_data = 10*mytid;
  reference_data = 10*left;

  call MPI_Sendrecv( my_data,1,MPI_INTEGER, right,0,&
		other_data,1,MPI_INTEGER, left,0,&
		comm,MPI_STATUS_IGNORE,err);
  if (other_data.ne.reference_data) then
    print *,"Error on process",mytid,"; received",other_data,"; expected",reference_data
 end if
10   call MPI_Finalize(err)

end program SendRecv
