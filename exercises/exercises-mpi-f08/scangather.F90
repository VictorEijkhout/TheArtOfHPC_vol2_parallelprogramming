! ****************************************************************
! ****
! **** This program file is part of the book 
! **** `Parallel programming for Science and Engineering'
! **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
! ****
! **** copyright Victor Eijkhout 2012-2020
! ****
! **** MPI Exercise for the use of Scan/Exscan
! ****
! ****************************************************************/

Program ScanGather

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors

  ! random stuff
  integer,allocatable,dimension(:) :: randseed
  integer :: randsize,irand
  real(4) :: randomfraction

  !!
  !! Local variables
  !!
  integer :: my_number_of_elements,my_first_index,total_number_of_elements,&
       i_element
  integer,dimension(:),allocatable :: my_elements,size_buffer,displ_buffer,&
       gather_buffer

  !!
  !! Setup MPI
  !!
  call MPI_Init()
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)
  
  !!
  !! Initialize the random number generator
  !! using the process number to get a unique seed
  !!
  call random_seed(size=randsize)
  allocate(randseed(randsize))
  do irand=1,randsize
     randseed(irand) = 1023*procno
  end do
  call random_seed(put=randseed)

  !!
  !! How many elements do I have locally?
  !! Allocate space.
  !!
  call random_number(randomfraction)
  my_number_of_elements = randomfraction * nprocs
  my_first_index=0

  !!
  !! Exercise:
  !! - determine the index of your first element.
  !!   Which routine do you use here?
  !!
!!!! your code here !!!!
    (my_number_of_elements,my_first_index, &
    1,MPI_INTEGER,MPI_SUM,comm)
  print '("Proc ",i3," has ",i3," elements, range [",i4,",",i4,")")', &
       procno,my_number_of_elements,my_first_index,my_first_index+my_number_of_elements

  !!
  !! Create a local array of size `my_number_of_elements'
  !! Fill in this local array
  !!
  allocate(my_elements(my_number_of_elements))
  do i_element=1,my_number_of_elements
     my_elements(i_element) = my_first_index+i_element-1
  end do

  call MPI_Reduce( my_number_of_elements,total_number_of_elements, &
       1,MPI_INTEGER,MPI_SUM,0,comm)
  if (procno==0) then
     print *,"Total number of elements: ",total_number_of_elements
  end if
  
  !!
  !! Gather all local arrays into one global
  !!

  !! how many elements from each process?
  if (procno==0) then
     allocate(size_buffer(nprocs))
     allocate(displ_buffer(nprocs))
  else 
     allocate(size_buffer(0))
     allocate(displ_buffer(0))
  end if
  call MPI_Gather &
       ( &
!!!! your code here !!!!
       )
  !! where are they going to go in the big buffer?
  call MPI_Gather &
       ( &
!!!! your code here !!!!
       )
  !! now create the big buffer
  if (procno==0) &
       allocate(gather_buffer(total_number_of_elements))

  !!
   !! Use Gatherv to collect the small buffers into a big one
   !!
  call MPI_Gatherv &
       ( &
!!!! your code here !!!!
       )

  !!
   !! Print the gathered material
   !!
  if (procno==0) &
     print '("Gathered:",100(1x,i0))', &
          ( gather_buffer(i_element),i_element=1,total_number_of_elements )

  call MPI_Finalize()
 
End Program ScanGather
