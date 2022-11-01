!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2022
!****
!**** OpenMP example of parametrized hello world.
!****
!****************************************************************/

Program Hello

  use omp_lib
  integer :: nthreads,mythread
  
!$omp parallel
  nthreads = omp_get_num_threads()
  mythread = omp_get_thread_num()
  write(*,'("Hello from",i3," out of",i3)') mythread,nthreads
!$omp end parallel

end Program Hello
