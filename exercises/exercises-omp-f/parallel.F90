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

Program CommRank

  use omp_lib

  integer :: sum,how_many_threads,sumcheck

  print *,"There are",omp_get_num_procs(),"cores available"
  
  !! Exercise part 1:
  !! -- fill in the routine that reports the nummber of threads.
!$omp parallel
  how_many_threads = &
!!!! your code here !!!!
  print *,"There are",how_many_threads,"active threads"
!$omp end parallel

  !! Exercise part 2:
  !! -- add the thread numbers
  !! -- run the program. run it again.
  !! -- run the program a number of times:
  !!    for i in `seq 1 100` ; do ./parallel ; done | grep dangers
  sum = 0
!$omp parallel
  sum = sum + &
!!!! your code here !!!!
!$omp end parallel

  sumcheck = how_many_threads*(how_many_threads-1)/2
  if (sum.eq.sumcheck) then
     print *,"You got lucky: sum is",sum
  else
     print *,"And here are the dangers of parallel programming: sum is",sum,&
          "; should be",sumcheck
  end if
  
end Program CommRank
