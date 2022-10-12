! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for vector type
!**** fortran 2008 version
!****
!****************************************************************/

Program StrideSend

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors

  !! data for this exercise:
  integer :: sender=0, localsize=10, ndata, sendto, i
  integer,dimension(:),allocatable :: data
  !! datatype handling
  Type(MPI_Datatype) :: scattertype
  integer :: count,stride,blocklength
  
  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  if (nprocs<2) then
     print *,"This program needs at least two processes"
     call MPI_Abort(comm,0)
  end if


  if (procno==sender) then
     !
     ! Create big data array to be send
     ! Notes: zero-based indexing
     !
     ndata = localsize * nprocs
     ALLOCATE( data(0:ndata-1),stat=ierr )
     if ( ierr/=0 ) then
        print *,"Out of memory"
        call MPI_Abort(comm,0)
     end if
     data = [ (i,i=0,ndata-1) ]
     
     !
     ! Exercise: define a datatype for sending:
     ! - set the variables count,stride,blocklength
     ! - use MPI_Type_vector to create the strided type
!!!! your code here !!!!

     !
     ! Loop over all processes you are sending to
     !
     do sendto=0,nprocs-1
        if (sendto==procno) cycle
        !
        ! Exercise: you could also define the datatype here. Why or why not?
        ! - do a send to the other processor
        !
!!!! your code here !!!!
     end do
     !
     ! Exercise:
     ! - you need to free the datatype. here or inside the send loop?
     !
!!!! your code here !!!!
  else
     !
     ! Create local array; note, again zero-based
     !
     ndata = localsize
     ALLOCATE( data(0:ndata-1),stat=ierr )
     if ( ierr/=0 ) then
        print *,"Out of memory"
        call MPI_Abort(comm,0)
     end if
     call MPI_Recv(data,localsize,MPI_INTEGER,sender,0,comm,MPI_STATUS_IGNORE)
     do i=0,localsize-1
        if ( data(i) /= i*nprocs+procno ) then
             write(6,'("[",i3,"] received element",i3,", should be",i3)') &
             procno,data(i),i*nprocs+procno
             exit
          end if
     end do
  end if
  
  if (procno==0) print *,"Finished"

  call MPI_Finalize();

end Program StrideSend
