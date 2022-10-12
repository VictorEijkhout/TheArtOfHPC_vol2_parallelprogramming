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

Program TaskFactor

  use omp_lib

  integer :: N = 2999*3001, factor = 0, f

  !! Exercise:
  !! -- turn the factor-testing block into a task
  !! -- run your program several times.
  !!    for i in `seq 1 1000` ; do ./taskfactor ; done | grep -v 2999
  !!    why does it find the wrong solution? try to fix this

  !$OMP parallel
  !$OMP single
  do f=2,4000
!!!! your code here !!!!
     !! see if `f' is a factor
     if (MOD(N,f).eq.0) then !! found factor!
!!!! your code here !!!!
         end if
      end if
!!!! your code here !!!!
      if (factor>0) goto 10
   end do
10 continue
   !$OMP end single
   !$OMP end parallel
   
   if (factor>0) then
      print *,"Found a factor:",factor
   else
      print *,"Prime number!"
   end if
 
end Program TaskFactor
 
