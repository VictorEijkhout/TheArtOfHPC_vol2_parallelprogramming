!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!
!!!! This program file is part of the book and course
!!!! "Parallel Computing"
!!!! by Victor Eijkhout, copyright 2013-2024
!!!!
!!!! ijwrongright.F90 : proper data handling
!!!!
!!!! NOTE for this example to go actually wrong, 
!!!! compile with OPTLEVEL=0
!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Program IJWrongRight
  use omp_lib
  implicit none

  ! arg handling
  integer :: argc
  character(len=12) :: a_matrix_size

  ! numerical parameters
  integer :: matrix_size, nex,i,j
  double precision,dimension(:,:),allocatable :: matrix
  double precision :: sum,tstart,duration

  ! arg parsing
  if (command_argument_count()==0) then
     matrix_size = 10000
  else
     call get_command_argument(1,a_matrix_size)
     read(a_matrix_size,*) matrix_size
  end if
  allocate( matrix(matrix_size,matrix_size) )

  !!
  !! Sequential for reference
  !!
  call matinit( matrix,matrix_size );
  tstart = omp_get_wtime();
  do nex=1,100
     do j=1,matrix_size
        do i=1,matrix_size
           matrix(i,j) = matrix(i,j) * 3.14
        end do
     end do
  end do
  duration = omp_get_wtime()-tstart;
  sum = matsum( matrix,matrix_size );
  print 10,"Sequential:",duration,sum

  !!
  !! The wrong way
  !!
  call matinit( matrix,matrix_size );
  tstart = omp_get_wtime();
  do nex=1,100
!$omp parallel do
     do j=1,matrix_size
        do i=1,matrix_size
           matrix(i,j) = matrix(i,j) * 3.14
        end do
     end do
!$omp end parallel do
  end do
  duration = omp_get_wtime()-tstart;
  sum = matsum( matrix,matrix_size );
  print 10,"Wrong time :",duration,sum

  !!
  !! The right way
  !!
  call matinit( matrix,matrix_size );
  tstart = omp_get_wtime();
  do nex=1,100
!$omp parallel do private(i)
     do j=1,matrix_size
        do i=1,matrix_size
           matrix(i,j) = matrix(i,j) * 3.14
        end do
     end do
!$omp end parallel do
  end do
  duration = omp_get_wtime()-tstart;
  sum = matsum( matrix,matrix_size );
  print 10,"Right time :",duration,sum

10 format(a12,x,f7.4,e12.5)

Contains
  Subroutine matinit( matrix,matrix_size ) 
    implicit none
    integer :: matrix_size
    double precision :: matrix(matrix_size,matrix_size)
    ! local
    integer :: i,j

    do j=1,matrix_size
       do i=1,matrix_size
          matrix(i,j) = 1./ (i+j+1);
       end do
    end do
  End Subroutine matinit

  Function matsum( matrix,matrix_size ) result(s)
    implicit none
    integer :: matrix_size
    double precision :: matrix(matrix_size,matrix_size)
    ! local
    integer :: i,j
    double precision :: s

    do j=1,matrix_size
       do i=1,matrix_size
          s = s + matrix(i,j)
       end do
    end do
  End Function matsum

  Subroutine noopt( matrix,matrix_size ) 
    implicit none
    integer :: matrix_size
    double precision :: matrix(matrix_size,matrix_size)
    ! local
    integer :: i,j
    double precision :: s

    s = matsum(matrix,matrix_size);
    if (s==0.) print *,s
  End Subroutine noopt
  
End Program IJWrongRight
