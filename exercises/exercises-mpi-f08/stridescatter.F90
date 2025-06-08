!!   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!!   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!!   %%%%
!!   %%%% This program file is part of the book and course
!!   %%%%   "Parallel Computing for Science and Engineering"
!!   %%%% by Victor Eijkhout, copyright 2013-2023
!!   %%%%
!!   %%%% MPI example for vector type and extent resizing
!!   %%%%
!!   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!!   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!!

Program StrideScatter

  use mpi_f08
  use iso_c_binding
  use tools
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno
  !! for this exercise
  integer :: sender=0, ndata, count,stride,blocklength, i
  logical :: error
  integer(kind=MPI_Address_kind) :: l,e
  integer,parameter :: localsize=10
  integer,dimension(localsize) :: mydata
  integer,dimension(:),allocatable :: data
  !! datatypes
  Type(MPI_Datatype) :: scattertype,interleavetype

  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  if (nprocs<2) then
     print *,"This program needs at least two processes"
     call MPI_Abort(comm,0)
  end if

  if (procno==sender) then
     !!
     !! Create big data array to be scattered
     !!
     ndata = localsize*nprocs
     ALLOCATE( data(0:ndata-1) )
     data = [ (i,i=0,ndata-1) ]

     !!
     !! Exercise
     !! - you need a vector datatype for sending data
     !! - resize its extent to achieve proper interleaving
     !!
     count = localsize; stride = nprocs; blocklength = 1
     call MPI_Type_vector(count,blocklength,stride,MPI_INTEGER,scattertype)
     call MPI_Type_commit(scattertype)
     !!solution
     call MPI_Type_get_extent(scattertype,l,e)
     e = c_sizeof(i)
     call MPI_Type_create_resized(scattertype,l,e,interleavetype)
     call MPI_Type_commit(interleavetype)
     !!solved
  end if

  call MPI_Scatter( data,1,interleavetype, & ! big send buffer
       mydata,localsize,MPI_INTEGER,       & ! small receive buffer
       sender,comm )

#if 1
  !!
  !! Trace output is disabled by default
  !! Change 0 -> 1 to get a full printout
  !! of the data of each process
  !!
  print '("[",i3,"]",20(x,i3))',procno,(mydata(i),i=1,localsize)
#endif

  !!
  !! Correctness check
  !!
  error = .false.
  do i=1,localsize
     if ( MOD( mydata(i),nprocs )/=procno ) then
        print '("Error: [",i3,"] received ",i3," s/b ",i3)',&
             procno,mydata(i),i*nprocs+procno
        error = .true.
     end if
  end do
  call print_final_result(error,comm)

  if (procno==sender) then
     call MPI_Type_free(interleavetype)
     call MPI_Type_free(scattertype)
     deallocate( data )
  end if

  call MPI_Finalize()

End Program StrideScatter
