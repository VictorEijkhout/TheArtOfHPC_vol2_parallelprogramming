!****************************************************************
!****
!**** hellocount.F90
!****
!****************************************************************

Program Hello

  use omp_lib
  integer :: nthreads,mythread
  
!$omp parallel
  nthreads = omp_get_num_threads()
  mythread = omp_get_thread_num()
  write(*,'("Hello from",i3," out of",i3)') mythread,nthreads
!$omp end parallel

end Program Hello
