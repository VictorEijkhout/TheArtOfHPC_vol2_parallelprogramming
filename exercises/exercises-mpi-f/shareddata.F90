! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise Fortran 90
!**** shareddata.F90 : MPI shared memory
!****
!****************************************************************/

Program SharedData
  use mpi
  USE, INTRINSIC :: ISO_C_BINDING, ONLY : C_PTR, &
       c_f_pointer 
  implicit none

  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  ! communicators
  integer :: node_comm,crosscomm
  integer :: onnode_procno, nodeid

  ! window stuff
  integer :: node_window
  integer :: displacement,window_elt_size
  integer(kind=MPI_ADDRESS_KIND) :: window_size,offset=0, &
       window_size0
  type(C_ptr) ::  win0_addr
  integer :: window_unit
  real*8,pointer :: win0_f_ptr

  ! other data for this exercise
  real*8 :: shared_value,check_val
  type(c_ptr) :: shared_data_ptr
  real*8,pointer :: shared_data_f_ptr

  ! error checking
  integer :: error,errors

  interface
     integer function c_contents(win_buffer) &
          bind(C,name="address_contents")
       use, intrinsic :: iso_c_binding
       type(c_ptr) :: win_buffer
     end function c_contents
  end interface

  call MPI_Init(ierr)
  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Exercise:
  !! -- construct `node_comm': the communicator that contains all processes on this node
  !!    the correct type is in the book/slides
  !! Find the subcommunicator on the node
  !! and get the procno on the node
  !!
  call MPI_Comm_split_type(comm, &
       !!solution
       MPI_COMM_TYPE_SHARED, &
       !!solved
       procno, MPI_INFO_NULL, node_comm,ierr)

  !!
  !! Exercise:
  !! -- compute the rank on the node
  call MPI_Comm_rank( &
       !!solution
       node_comm, &
       !!solved
       onnode_procno,ierr)

  !!
  !! Exercise:
  !! -- make subcommunicators with one rank per node each
  !!    the procno in that communicator should be the node ID
  !!
  call MPI_Comm_split( &
       !!solution
       comm,onnode_procno, &
       !!solved
       procno,crosscomm,ierr)
  call MPI_Comm_rank(crosscomm,nodeid,ierr);
  write(0, '("[",i3,"] has node,onnode coordinate ",i3,",",i3)' ) &
       procno,nodeid,onnode_procno
  flush(0)

  !!
  !! Create data on global process zero and broadcast it to process zero on other nodes
  !!
#define REFVAL 3.14
  shared_value = REFVAL
  if (onnode_procno==0) then
     call MPI_Bcast(shared_value,1,MPI_REAL8,0,crosscomm,ierr)
     write(0, '("[",i3,"] Head nodes now have shared data: ",f9.5)' ) &
          procno,shared_value
     flush(0)
  end if
  
    
  !!
  !! Exercise:
  !! -- Create window on the node communicator;
  !!    it should have nonzero size only on the first process
  !! -- What happens if you use MPI_Win_allocate?
  !!
  call MPI_Sizeof(shared_value,window_elt_size,ierr) ! non-optional ierr
  displacement = window_elt_size
  if (onnode_procno==0) then
     window_size = 1
  else
     window_size = 0
  end if
  call MPI_Win_allocate_shared( &
       window_size,displacement, MPI_INFO_NULL, &
       !!solution
       node_comm, &
       !!solved
       shared_data_ptr,node_window,ierr)
  call c_f_pointer(shared_data_ptr,shared_data_f_ptr)

  !!
  !! Put data on process zero of the node window
  !! We use a Put call rather than a straight copy:
  !! the Fence calls enforce coherence
  !!
  call MPI_Win_fence(0,node_window,ierr)
  if (onnode_procno==0) then
     offset = 0
     call MPI_Put( shared_value,1,MPI_DOUBLE, & ! local data
          0,offset,1,MPI_DOUBLE,              & ! location in local node window
          node_window,ierr)
  end if
  call MPI_Win_fence(0,node_window,ierr)

  !!
  !! Now get on all processes the address of the window of process zero.
  !!
  call MPI_Win_shared_query( node_window,0, &
       window_size0,window_unit, win0_addr ,ierr)
  call c_f_pointer(win0_addr,win0_f_ptr)
  check_val = win0_f_ptr !! c_contents(win0_addr)
  print *,procno,check_val

  !!
  !! Check that we can indeed get at the data in the shared memory 
  !!
  if ( abs(check_val-REFVAL)/REFVAL > 1.e-14 ) then
     error = procno
  else
     error =  nprocs
  end if
  errors=-1
  call MPI_Allreduce(error,errors,1,MPI_INTEGER,MPI_MIN,comm,ierr)
  if (procno==0) then
     if (errors==nprocs) then
        print *,"Finished; all results correct"
     else
        print *,"First error occurred on proc ",errors
     end if
  end if

  !!
  !! Cleanup
  !!
  call MPI_Win_free(node_window,ierr)
  call MPI_Finalize(ierr)
  
end Program SharedData

