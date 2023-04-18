!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2023
!****
!**** OpenMP example of hello world
!****
!****************************************************************/

Program Hello
  use omp_lib
  implicit none

  integer :: mythread,nthreads

!!codesnippet helloompf
!$omp parallel private(mythread)
  mythread = omp_get_thread_num()
  print *,"Hello world from",mythread
!$omp end parallel
!!codesnippet end

end Program Hello
