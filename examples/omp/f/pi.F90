!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!! %%%%
!! %%%% This program file is part of the book and course
!! %%%% "Parallel Computing"
!! %%%% by Victor Eijkhout, copyright 2013-2024
!! %%%%
!! %%%% pi.F90 : compute pi
!! %%%%
!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Program PiIntegrate

  use omp_lib
  implicit none

  integer,parameter :: N=10000
  real,parameter :: pi=3.14159265358979323
  real :: h = (1./N), pi4=0,err
  integer :: isample,nt
  real :: xsample,y

  !!  double tstart = omp_get_wtime();
  !!codesnippet pi4reductf
  !$omp parallel do reduction(+:pi4) private(xsample,y)
  do isample=0,N-1
     xsample = isample * h
     y = sqrt(1-xsample*xsample)
     pi4 = pi4 + h*y
  end do
  !!codesnippet end
  !! double duration = omp_get_wtime()-tstart

 err = pi-4*pi4
 !$omp parallel
 !$omp master
 nt = omp_get_num_threads()
 !$omp end master
 !$omp end parallel
 print *,"Pi computed with",N,"samples, ",nt,"threads: err=",err

end Program PiIntegrate
