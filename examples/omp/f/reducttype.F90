!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2021
!****
!**** OpenMP example of reduction on type
!****
!****************************************************************/

Module Typedef
  Type inttype
     integer :: value = 0
  end type inttype
  Interface operator(+)
     module procedure addints
  end Interface operator(+)
contains
  function addints(i1,i2) result(isum)
    implicit none
    Type(inttype),intent(in) :: i1,i2
    Type(inttype) :: isum
    isum%value = i1%value+i2%value
  end function addints
end Module Typedef

Program Reducttype
  use Typedef
  implicit none
  Type(inttype) :: i1,i2,i3
  integer,parameter :: nsize = 500
  Type(inttype),dimension(nsize) :: intarray
  Type(inttype) :: intsum = inttype(0)
  integer :: i

  i1 = inttype(1); i2 = inttype(2)
  i3 = i1+i2
  print *,"Sum:",i3%value

  do i=1,nsize
     intarray(i) = inttype(i)
  end do

  !$OMP parallel for reduction(+:intsum)
  do i=1,nsize
     intsum = intsum + intarray(i)
  end do
  !$OMP end parallel

  print '("Sum = ",i6,"; should be ",i6)',intsum,nsize*(nsize+1)/2
  
end Program Reducttype
