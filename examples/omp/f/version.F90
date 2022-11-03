!****************************************************************
!****
!**** version.F90 : deduct OpenMP version
!****
!****************************************************************

Program Hello

  use omp_lib
  implicit none
  integer :: standard

  standard = openmp_version

  select case (standard)
  case  (201511)
     print *,"4.5"
  case (201611)
     print *,"Technical report 4: information about 5.0 but not yet mandated."
  case (201811)
     print *,"5.0"
  case (202011)
    print *,"5.1"
 case (202111)
    print *,"5.2"
 case default
    print *,"Unrecognized standard:",standard
  end select

end Program Hello
