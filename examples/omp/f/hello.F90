!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2022
!****
!**** OpenMP example of hello world
!****
!****************************************************************/

Program Hello

!$omp parallel
  print *,"Hello world!"
!$omp end parallel

    ! int mythread,nthreads;
    ! nthreads = omp_get_num_threads();
    ! mythread = omp_get_thread_num();
    ! printf("Hello from %d out of %d\n",mythread,nthreads);

end Program Hello
