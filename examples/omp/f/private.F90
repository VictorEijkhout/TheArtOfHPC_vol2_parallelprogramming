!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2022
!****
!**** OpenMP example of private data
!****
!****************************************************************/

Program private
  use omp_lib
  implicit none
  integer :: x,t

  x=5
!$omp parallel private(x,t) num_threads(4)
  t = omp_get_thread_num()
  x = t+1
  print '("Thread ",i2," sets x to ",i2)',t,x
!$omp end parallel
  print '("Outer x is still ",i2)',x

End Program private
