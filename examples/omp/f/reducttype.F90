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
  !!codesnippet ftypeopdef
  Type inttype
     integer :: value = 0
  end type inttype
  Interface operator(+)
     module procedure addints
  end Interface operator(+)
  !!codesnippet end
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
  !!codesnippet ftypeopuse
  Type(inttype),dimension(nsize) :: intarray
  Type(inttype) :: intsum = inttype(0)
  !!codesnippet end
  integer :: i

  i1 = inttype(1); i2 = inttype(2)
  i3 = i1+i2
  print *,"Sum:",i3%value

  do i=1,nsize
     intarray(i) = inttype(i)
  end do

  !!codesnippet ftypeopuse
  !$OMP parallel do reduction(+:intsum)
  do i=1,nsize
     intsum = intsum + intarray(i)
  end do
  !$OMP end parallel
  !!codesnippet end

  print '("Sum = ",i6,"; should be ",i6)',intsum,nsize*(nsize+1)/2
  
end Program Reducttype
