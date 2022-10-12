! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI example of Fortrna array section sending
!****
!****************************************************************/

Program F90Section
  use mpi_f08
  implicit none

  integer :: i,j, nprocs,procno
  integer,parameter :: siz=20
  real,dimension(siz,siz) :: matrix = [ ((j+(i-1)*siz,i=1,siz),j=1,siz) ]
  real,dimension(2,2) :: submatrix
  Type(MPI_Comm) :: comm

  call MPI_Init()
  comm = MPI_COMM_WORLD

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  if (nprocs<2) then
     print *,"This example really needs 2 processors"
     call MPI_Abort(comm,0)
  end if
  if (procno==0) then
     call MPI_Send(matrix(1:2,1:2),4,MPI_REAL,1,0,comm)
  else if (procno==1) then
     call MPI_Recv(submatrix,4,MPI_REAL,0,0,comm,MPI_STATUS_IGNORE)
     if (submatrix(2,2)==22) then
        print *,"Yay"
     else
        print *,"nay...."
     end if
  end if

  call MPI_Finalize()

end Program F90Section
