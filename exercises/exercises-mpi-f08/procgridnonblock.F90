! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2023
!****
!**** MPI Exercise for multiple non-blocking collectives
!**** fortran 2008 version
!****
!****************************************************************/

Program ProcGridNonBlock

  use mpi_f08
  implicit none

  integer :: nprocs, procno
  type(MPI_Comm) :: comm = MPI_COMM_WORLD

  !! data specifically for this program
  integer :: &
    nrows,ncols, &
    row_rank,col_rank, row_no,col_no, &
    row_length, col_length
  type(MPI_Comm) :: row_comm,col_comm;
  type(MPI_Request),dimension(2) :: requests

  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  !!
  !! Try to arrange the processors in a grid
  !!
  do nrows=int(sqrt(nprocs+1.)),1,-1
     if (MOD(nprocs,nrows)==0) then
        goto 10
     end if
  end do
10 continue

  if (nrows==1) then
     if (procno==0) then
        print *,"Number of processes is prime"
        call MPI_Abort(comm,0)
     end if
  end if
  ncols = nprocs/nrows;

  !!
  !! Find the coordinates of this process
  !!
  row_no = procno/ncols;
  col_no = procno - ncols*row_no;

  !!
  !! Make a `col_comm' communicator with all processes in this column
  !!
  call MPI_Comm_split(comm, col_no,0,col_comm)
  !!
  !! Make a `row_comm' communicator with all processes in this row
  !!
  call MPI_Comm_split(comm, row_no,0,row_comm)

  !!
  !! Now find `row_rank': the number I am in my row
  !! by using MPI_Comm_rank
  !!
  call MPI_Comm_rank(row_comm,row_rank)
  !!
  !! also find `col_rank': the number I am in my column
  !! by using MPI_Comm_rank
  !!
  call MPI_Comm_rank(col_comm,col_rank)

  !!
  !! Let only the left column and top row query the length
  !! of a row / column respectively
  !!
  if (row_rank==0) &
       call MPI_Comm_size(row_comm,row_length)
  if (col_rank==0) &
       call MPI_Comm_size(col_comm,col_length)

  !!
  !! Exercise:
  !! - the left column and top row tell everyone else
  !!   what these lengths are
  !!
#if 0
  !! replace these blocking calls by non-blocking
  call MPI_Bcast( row_length,1,MPI_INTEGER,0,row_comm )
  call MPI_Bcast( col_length,1,MPI_INTEGER,0,col_comm )
#endif
  !!solution
  call MPI_Ibcast( row_length,1,MPI_INTEGER,0,row_comm,requests(1) )
  call MPI_Ibcast( col_length,1,MPI_INTEGER,0,col_comm,requests(2) )
  call MPI_Waitall( 2,requests,MPI_STATUSES_IGNORE )
  !!solved

  
  if (procno==0) then
     print *,"Finished"
  endif
 
  call MPI_Finalize()
  
end Program ProcGridNonBlock
