! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-6
!****
!**** MPI Exercise 
!****
!****************************************************************/

Program ProcGrid
  implicit none

#include <mpif.h>

  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  !! data specifically for this program
  integer :: &
    nrows,ncols, &
    row_no,col_no, &
    row_rank,col_rank
  integer :: row_comm,col_comm;

  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

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
        call MPI_Abort(comm,0,ierr)
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
  call MPI_Comm_split(comm, &
       !!solution
       col_no,0,col_comm, &
       !!solved		 
       ierr)
  !!
  !! Make a `row_comm' communicator with all processes in this row
  !!
  call MPI_Comm_split(comm, &
       !!solution
       row_no,0,row_comm, &
       !!solved		 
       ierr)

  !!
  !! Now find `row_rank': the number I am in my row
  !! by using MPI_Comm_rank
  !!
  call MPI_Comm_rank( &
       !!solution
       row_comm,row_rank, &
       !!solved
       ierr)
  !!
  !! also find `col_rank': the number I am in my column
  !! by using MPI_Comm_rank
  !!
  call MPI_Comm_rank( &
       !!solution
       col_comm,col_rank, &
       !!solved
       ierr)

  !  write(*,'("Process",i3," is (",i2,",",i2,")")') procno,col_rank,row_rank

  !!
  !! Now check that the rank in the row equals the column number
  !!
  if (row_rank/=col_no) then
     print *,"wrong row rank",row_rank
  end if
  !!
  !! Now check that the rank in the column equals the row number
  !!
  if (col_rank/=row_no) then
     print *,"wrong col rank",col_rank
  end if

  if (procno==0) then
     print *,"Finished"
  endif
 
  call MPI_Finalize(ierr)
  
end Program 
