! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for I/O
!****
!****************************************************************/

Program BlockWrite
  implicit none
#include <mpif.h>

  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  !! variables for this particular exercise
  integer :: nwords,ip,iw,nwriters,location
  integer*4 :: fromfile
  integer :: sizeofint=4, mpiunit=11
  integer,allocatable,dimension(:) :: output_data
  integer :: mpifile ! there is no MPI_File in Fortran
  integer(kind=MPI_OFFSET_KIND) :: offset

  !! regression testing
  integer :: error,errors

  call MPI_Init(ierr)
  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  nwriters = nprocs-1
  nwords = 3

  !!
  !! Each process has a buffer of length `nwords'
  !! containing consecutive integers
  !!
  ALLOCATE( output_data(nwords) )
  do iw=1,nwords
     output_data(iw) = procno*nwords+iw
  end do

  !!
  !! Exercise:
  !! -- open a file for writing
  !!    if it doesn't exist yet, it needs to be created
  !!
  call MPI_File_open(comm,"blockwrite.dat",&
!!!! your code here !!!!
       MPI_INFO_NULL,&
       mpifile,ierr)
  
  !!
  !! Exercise:
  !! -- compute an offset (in bytes!) so that processes write
  !!    to disjoint parts of the file
  !!
  if (procno<nwriters) then
     offset = &
!!!! your code here !!!!
     call MPI_File_write_at( &
          mpifile,offset,output_data,nwords, &
          MPI_INT,MPI_STATUS_IGNORE,ierr)
  end if
  call MPI_File_close(mpifile,ierr)

  
  !!
  !! Check correctness of the output file
  !!
  if (procno==0) then
     error = nprocs
     open(mpiunit,file="blockwrite.dat",action="read",&
          access="stream",form="unformatted")
     write(6,'("Trying to read",i4," integers")') nwriters*nwords
     location = 0
     readloop: do ip=1,nwriters
        do iw=1,nwords
           read(mpiunit,err=998,end=999) fromfile
           if (fromfile.ne.location+1) then
              print *,"Found",fromfile,"; should be",location+1
           end if
           location = location+1
           cycle

998        print *,"Error after",location,"ints"
           error = procno
           exit readloop
999        print *,"End of file after",location,"ints"
           error = procno
           exit readloop
        end do
     end do readloop
     close(mpiunit)
     errors = error
     if (errors==nprocs) then
        print *,"Execution finished correctly: read and validated",location," integers"
     else
        print *,"Execution finished with errors"
     end if
  end if
  
  call MPI_Finalize(ierr)

end Program BlockWrite
