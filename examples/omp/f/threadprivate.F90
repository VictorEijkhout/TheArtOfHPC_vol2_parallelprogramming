!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2022
!****
!**** OpenMP example of threadprivate data
!****
!****************************************************************/

Program Private

  use omp_lib
  implicit none

  common /threaddata/tp
  integer :: tp
!$omp threadprivate(/threaddata/)

!$omp parallel num_threads(7)
  tp = omp_get_thread_num()
!$omp end parallel

!$omp parallel num_threads(9)
  print *,"Thread",omp_get_thread_num(),"has",tp
!$omp end parallel

end Program Private
