! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise to illustrate graph topologies
!**** fortran 2008 version
!****
!****************************************************************

Program RightGraph

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors

  !! data for this exercise:
  type(MPI_Comm) :: pair_communicator = MPI_COMM_NULL
  real*8, dimension(2) :: leftandme
  real*8 :: mydata, & !! my data that will be sent right
       leftdata=0,  & !! container for data received from left
       check
  !!
  !! Exercise:
  !! -- declare arrays for the local description of the topology
  !!
  integer nsources
  integer,dimension( &
!!!! your code here !!!!
       ) :: sources,destinations,degrees,weights

  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  mydata = procno

  !!
  !! Exercise:
  !! -- fill in the arrays that describe the topology
  !! .. set nsources
  !! .. fill the arrays `sources', `destinations', `degrees'
  !! .. for the weights use 1 everywhere
  nsources = &
!!!! your code here !!!!

  call MPI_Dist_graph_create( comm, &
       nsources,     & ! number of sources declared here
       sources,      & ! where does our data come from?
       degrees,      & ! degrees of the sources
       destinations, & ! where does our data go to
       weights, MPI_INFO_NULL,.FALSE., &
       pair_communicator, &
       ierr )

  call MPI_Neighbor_allgather( &
       mydata,1,MPI_DOUBLE, &
       leftandme,1,MPI_DOUBLE, &
       pair_communicator,ierr )
  leftdata = leftandme(1)

  !!
  !! Check correctness
  !!
  if (procno.gt.0 .and. leftdata/=mydata-1) then
     error = procno
  else
     error = nprocs
  end if
  call MPI_Allreduce(error,errors,1,MPI_INTEGER,MPI_MIN,comm)
  if (procno.eq.0) then
     if (errors.eq.nprocs) then
        print *,"Finished; all results correct"
     else
        print *,"First error occurred on proc",errors
     end if
  end if

  call MPI_Finalize()
  
end Program RightGraph
