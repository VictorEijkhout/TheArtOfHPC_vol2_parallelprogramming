! /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%
!    %%%% This program file is part of the book and course
!    %%%% "Parallel Computing"
!    %%%% by Victor Eijkhout, copyright 2013-6
!    %%%%
!    %%%% struct.c : illustrating type struct
!    %%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! */

Program StructType

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm;
  integer :: nprocs,procno;

  !!
  !! The Type that we are going to send
  !!
  Type object 
     character :: c
     real*8,dimension(2) :: x
     integer :: i
  end type object

#define MPI_Aint integer(kind=MPI_ADDRESS_KIND)

  !!
  !! local data
  !!
  integer :: sender=0,receiver=1
  type(object) :: myobject
  integer,parameter :: structlen = 3
  type(MPI_Datatype) :: newstructuretype
  integer,dimension(structlen) :: blocklengths
  type(MPI_Datatype),dimension(structlen) :: types;
  MPI_Aint,dimension(structlen) :: displacements
  MPI_Aint :: base_displacement, next_displacement

  !!
  !! Initial setup
  !!
  call MPI_Init()
  comm = MPI_COMM_WORLD;
  call MPI_Comm_rank(comm,procno)
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN)

  if (nprocs<2) then 
     print *,"This program needs at least two processes"
     call MPI_Abort(comm,1)
  end if

  !!
  !! Fill in meaningful data only on the sender
  !!
  if (procno==sender) then
     myobject%c = 'x'
     myobject%x(0) = 2.7; myobject%x(1) = 1.5
     myobject%i = 37

     print '("Set: char=",a1,x,", double0=",x,f9.5,x,"double1=",x,f9.5,x,", int=",i5)', &
          myobject%c,myobject%x(0),myobject%x(1),myobject%i
  else
     myobject%c = ' '
     myobject%x(0) = 0.0; myobject%x(1) = 0.0
     myobject%i = 0
  end if

  !!
  !! Where are the components relative to the structure?
  !!

  !! component 1: one character
  blocklengths(1) = 1; types(1) = MPI_CHAR
  call MPI_Get_address(myobject,base_displacement)
  call MPI_Get_address(myobject%c,next_displacement)
  displacements(1) = next_displacement-base_displacement

  !! component 2: two doubles
  blocklengths(2) = 2; types(2) = MPI_DOUBLE
  call MPI_Get_address(myobject%x,next_displacement)
  displacements(2) = next_displacement-base_displacement

  !! component 3: one int
  blocklengths(3) = 1; types(3) = MPI_INT
  call MPI_Get_address(myobject%i,next_displacement)
  displacements(3) = next_displacement-base_displacement

  if (procno==sender) then
     print '("Displacements:",3(1x,i0))',displacements
  end if

  !!
  !! Create the structure type
  !!
  call MPI_Type_create_struct(structlen,blocklengths,displacements,types,newstructuretype)
  call MPI_Type_commit(newstructuretype)

  !!
  !! Send and receive call, both using the structure type
  !!
  if (procno==sender) then
     call MPI_Send(myobject,1,newstructuretype,receiver,0,comm)
  else if (procno==receiver) then
     call MPI_Recv(myobject,1,newstructuretype,sender,0,comm,MPI_STATUS_IGNORE)
  end if
  call MPI_Type_free(newstructuretype)

  !!
  !! Print out what we sent and received
  !!
  if (procno==sender) then
     print '("Sent: char=",a1,x,", double0=",x,f9.5,x,"double1=",x,f9.5,x,", int=",i5)', &
          myobject%c,myobject%x(0),myobject%x(1),myobject%i
  else if (procno==receiver) then
     print '("Received: char=",a1,x,", double0=",x,f9.5,x,"double1=",x,f9.5,x,", int=",i5)', &
          myobject%c,myobject%x(0),myobject%x(1),myobject%i
  end if

  if (procno==0) then
     print *,"Finished"
  end if

  call MPI_Finalize()


end Program StructType
