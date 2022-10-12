!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-7
!****
!**** OpenMP Exercise 
!****
!****************************************************************/

!! N is 10 million
#define N 10000000

Program PiAdapt

  use omp_lib

  double precision :: h = 1./(N+1), t,pi,quarterpi
  double precision :: x,x2,y,y2, xs,ys,hs, slope
  integer :: nsamples,samples,is,i

  !! start timer
  t = omp_get_wtime()

  quarterpi = 0.; nsamples = 0;

  !! Exercise 1:
  !! -- make the for loop parallel
  !! Eercise 2:
  !! -- try out different loop schedules:
  !!    static: this gives a baseline performance. speedup should be less than linear.
  !!    static with chunksize : why may this help?
  !!    dynamic: try different chunk sizes
  !!    guided: does this improve over dynamic?

  !$omp parallel &
      private(x,x2,y,y2,slope, xs,hs,ys, is,samples) shared(nsamples) &
      reduction(+:quarterpi)
  !$omp do
  do i=0,N-1
     x = i*h; x2 = (i+1)*h
     y = sqrt(1-x*x); y2 = sqrt(1-x2*x2)
     slope = (y-y2)/h
     if (slope>15) slope = 15;
     samples = 1+slope
     do is=0,samples-1
        hs = h/samples; xs = x+ is*hs; ys = sqrt(1-xs*xs);
        quarterpi = quarterpi + hs*ys;
        nsamples = nsamples+1
     end do
  end do
!!!! your code here !!!!
  pi = 4*quarterpi;

  !$omp end parallel

  !! stop timer and report
!$omp parallel
  t = omp_get_wtime()-t
  if (omp_get_thread_num()==0) then
     print *,"Pi=",4*quarterpi,"in time on",omp_get_num_threads(),"threads:",t
  end if
!$omp end parallel
  
end Program PiAdapt
