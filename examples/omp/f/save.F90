!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2022
!****
!**** OpenMP example of data with SAVE attribute
!****
!****************************************************************/

Program Hello

  use omp_lib
  integer :: nthreads,mythread
  
!$omp parallel
  call savehello
  call finehello
!$omp end parallel

contains

  subroutine savehello
    use omp_lib
    implicit none
    integer :: thread = -1
    thread = omp_get_thread_num()
    print *,"Hello from",thread
  end subroutine savehello
  subroutine finehello
    use omp_lib
    implicit none
    integer :: thread
    thread = omp_get_thread_num()
    print *,"World from",thread
  end subroutine finehello


end Program Hello
