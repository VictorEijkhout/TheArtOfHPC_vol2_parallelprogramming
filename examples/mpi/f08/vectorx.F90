!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!! %%%%
!! %%%% This program file is part of the book and course
!! %%%% "Parallel Computing"
!! %%%% by Victor Eijkhout, copyright 2013-2021
!! %%%%
!! %%%% vectorx.F90 : sending large amounts of data
!! %%%%
!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!!

Program VectorX
  use mpi_f08
  implicit none

  real*4,dimension(:),allocatable :: source,target
  integer :: mediumsize,nblocks
  integer :: sender,receiver,the_other,count,stride
  Type(MPI_Datatype) :: blocktype
  Type(MPI_Status) :: recv_status
  Integer(kind=MPI_COUNT_KIND) :: recv_count

#include "globalinit.F90"

  if (ntids<2) then 
     print *,"This program needs at least two processes"
     stop -1
  end if

  sender = 0; receiver = 1; the_other = 1-mytid; count = 5; stride=2

  mediumsize = 100000000 !! 100 million
  nblocks = 8

  if (mytid==sender) then
     allocate(source(nblocks*mediumsize))
    ! long int idx = 0;
    ! for (int iblock=0; iblock<nblocks; iblock++) {
    !   for (int element=0; element<mediumsize; element++) {
    !     source[idx] = idx+.5; idx++;
    !     if (idx<0) { printf("source index wrap\n"); call MPI_Abort(comm,0); }
    !   }
    ! }
 end if

  if (mytid==receiver) then
     allocate(target(nblocks*mediumsize))
   end if


  call MPI_Type_contiguous(mediumsize,MPI_REAL4,blocktype)
  call MPI_Type_commit(blocktype)
  if (mytid==sender) then
    call MPI_Send(source,nblocks,blocktype,receiver,0,comm);
  else if (mytid==receiver) then
    call MPI_Recv(target,nblocks,blocktype,sender,0,comm,recv_status)
    call MPI_Get_elements_x(recv_status,MPI_REAL4,recv_count)
    !!printf("Received %7.3f medium size elements\n",recv_count * 1e-9);
 end if
  call MPI_Type_free(blocktype)
  
  ! if (0 && mytid==receiver) {
  !   for (int i=0; i<count; i++)
  !     if (target[i]!=source[stride*i])
  !       printf("location %d %e s/b %e\n",i,target[i],source[stride*i]);
  ! }

  if (mytid==0) &
    print *,"Finished"

  call MPI_Finalize()

end Program VectorX


