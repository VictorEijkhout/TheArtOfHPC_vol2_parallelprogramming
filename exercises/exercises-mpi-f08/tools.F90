!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2021
!****
!**** tools for MPI exercises
!**** fortran 2008 version
!****
!****************************************************************/

Module Tools
contains

  !!
  !! elementwise max relative error between two arrays
  !!
  real(8) function array_error( ref_array,val_array ) result(error)
    implicit none
    real(8),dimension(:),intent(in) :: ref_array,val_array
    real(8) :: max_value, min_value, ref,val,rel
    integer :: i

    error = 0; max_value = -1; min_value = -1
    do i=1,size(ref_array)
       ref = ref_array(i); val = val_array(i)
       if ( min_value<0 .or. val<min_value ) min_value = val
       if ( max_value<0 .or. val>max_value ) max_value = val
       rel = abs( (ref-val)/ref )
       error = max(rel,error)
    end do

  end function array_error

  !!
  !! print overall error result
  !! input: cond, boolean if error occurred
  !!
  subroutine print_final_result( cond,comm )
    use mpi_f08
    implicit none
    logical,intent(in) :: cond
    Type(MPI_Comm),intent(in) :: comm
    integer :: nprocs,procno, error,errors

    call MPI_Comm_size(comm,nprocs)
    call MPI_Comm_rank(comm,procno)
    if (cond) then
       error = procno
    else
       error = nprocs
    end if
    call MPI_Allreduce( error,errors,1,MPI_INTEGER,MPI_MIN,comm)
    if (procno==0) then
       if (errors==nprocs) then
          print *,"Finished; all results correct"
       else
          print *,"First error occurred on proc",errors
       end if
    end if

  end subroutine print_final_result

End Module Tools

module posix
    use, intrinsic :: iso_c_binding, only: c_int, c_int32_t
    implicit none

    interface
        ! int usleep(useconds_t useconds)
        function c_usleep(useconds) bind(c, name='usleep')
            import :: c_int, c_int32_t
            integer(kind=c_int32_t), value :: useconds
            integer(kind=c_int)            :: c_usleep
        end function c_usleep
    end interface
end module posix
