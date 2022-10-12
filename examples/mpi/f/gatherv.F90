!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course 
!  %%%% "Parallel Computing for Science and Engineering"
!  %%%% by Victor Eijkhout, copyright 2013-2022
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program Gatherv

  use mpi
  implicit none

  integer :: localsize,root
  integer,dimension(:),allocatable :: localsizes,offsets,localdata,alldata

#include "globalinit.F90"

  root = ntids-1
  localsize =  10+10*( random_value-0.5 )

  ! create local data
  ALLOCATE( localdata(localsize) )

  do i=1,localsize
     localdata(i) = mytid+1
  end do
  ! we assume that each process has an array "localdata"
  ! of size "localsize"

  ! the root process decides how much data will be coming:
  ! allocate arrays to contain size and offset information
  if (mytid==root) then
    ALLOCATE( localsizes(ntids+1) )
    ALLOCATE( offsets(ntids) )
  end if
  ! everyone contributes their info
  call MPI_Gather(localsize,1,MPI_INTEGER,&
       localsizes,1,MPI_INTEGER,root,comm,err)
  if (mytid==root) then
     write(6,'("Local sizes: ",20(i3))') (localsizes(i),i=1,ntids)
  end if
  ! the root constructs the offsets array
  if (mytid==root) then
    offsets(1) = 0;
    do i=1,ntids
       offsets(i+1) = offsets(i)+localsizes(i)
    end do
    ALLOCATE( alldata(offsets(ntids+1)) )
  end if
  ! everyone contributes their data
  call MPI_Gatherv(localdata,localsize,MPI_INTEGER,&
	      alldata,localsizes,offsets,MPI_INTEGER,root,comm,err)
  ! if (mytid==root) then
  !   int p=0;
  !   printf("Collected:\n");
  !   for (i=0; i<ntids; i++) {
  !     int j;
  !     printf("  %d:",i);
  !     for (j=0; j<localsizes(i)-1; j++)
  !       printf("%d,",alldata[p++]);
  !     j=localsizes(i)-1;
  !     printf("%d;\n",alldata[p++]);
  !   }
  ! end if

  call MPI_Finalize(err)

end program Gatherv
