! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-6
!****
!**** MPI Exercise for the subarray type
!****
!****************************************************************/

Program CubeGather
  implicit none

#include <mpif.h>

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target

  !! data for this exercise:
  integer :: procs_per_side, proci,procj,prock, sender,i,j,k, error,mydata
  integer :: send_request, insert_type
  integer,dimension(:),allocatable :: cubedata
  integer :: sides(3),sub_sizes(3),startpoint(3)
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  
  !!
  !! See if you can arrange the processes
  !! in a perfect cube. If not, quit.
  !!
  procs_per_side=1
  do
     if (procs_per_side*procs_per_side*procs_per_side==nprocs) &
          exit;
     if (procs_per_side*procs_per_side*procs_per_side>nprocs) then
        if (procno==0) &
             print *,"Number of processes is not a cube"
        call MPI_Abort(comm,0,ierr)
     end if
     procs_per_side = procs_per_side+1
  end do
  
  !!
  !! Every process gets its own data, and sends it to process zero.
  !! Exercise: why are we using Isend here?
  !!
  mydata = procno+1
  call MPI_Isend(mydata,1,MPI_INT,0,0,comm,send_request,ierr)

  !!
  !! Now process zero receives all the contributions using the subarray type
  !! (You could use a gather here, but only because everyone sends just one element)
  !!
  if (procno==0) then
     allocate(cubedata(nprocs))
     do proci=0,procs_per_side-1
        do procj=0,procs_per_side-1
           do prock=0,procs_per_side-1
              !!
              !! Setup a subarray type describing where the data from
              !! process (proci,procj,prock) goes in the cube
              !!
!!!! your code here !!!!
              call MPI_Type_create_subarray( 3,sides,sub_sizes,startpoint, &
                   MPI_ORDER_FORTRAN,MPI_INT, insert_type, ierr)
              call MPI_Type_commit(insert_type,ierr)
              !! 
              !! Now the receive call.
              !!
              sender = ( prock*procs_per_side + procj )*procs_per_side +proci
              call MPI_Recv( &
!!!! your code here !!!!
                   sender,0,comm,MPI_STATUS_IGNORE,ierr)
              call MPI_Type_free(insert_type,ierr)
           end do
        end do
     end do
     write(*,*) "Received cube:"
     write(*,*) &
          (((cubedata(i + procs_per_side*( j-1 + procs_per_side*(k-1) ) ),&
                  i=1,procs_per_side),j=1,procs_per_side),k=1,procs_per_side)
     error = 0
     do i=1,nprocs
        if (cubedata(i)/=i) error = error+1
     end do
     if (error==0) then
        write(*,*) "Finished. Cube correctly gathered"
     else
        write(*,*) "There were",error,"errors"
     end if
  end if
  
  call MPI_Wait(send_request,MPI_STATUS_IGNORE,ierr)

  call MPI_Finalize(ierr)
  
end Program CubeGather
