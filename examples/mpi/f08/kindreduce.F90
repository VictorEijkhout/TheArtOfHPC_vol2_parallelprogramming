Module testsum
  implicit none
  Integer, Parameter :: dp = Selected_Real_Kind(15,307)

Contains

  subroutine Test(comm, vec, sumvalue)
    use mpi_f08
    implicit none
    type(MPI_Comm),intent(in) :: comm
    integer :: rank, ntask, ierr, stat
    integer :: I, N
    integer, allocatable :: workload(:)
    real (kind=dp), intent(in) :: vec(:)
    Real (kind=dp), intent(out) :: sumvalue
    real (kind=dp) :: si
    integer :: istart, iend
    Type(MPI_Datatype) :: mpi_dp
    call MPI_Comm_size(comm, ntask, ierr)
    call MPI_Comm_rank(comm, rank, ierr)
    Call MPI_TYPE_CREATE_F90_Real(15, 307, mpi_dp, ierr)
    N = size(vec)
    Allocate(workload(ntask))
    workload = N / ntask
    workload(1:mod(N,ntask)) = workload(1:mod(N,ntask)) + 1
    istart = sum(workload(1:rank))
    iend = istart + workload(rank+1)
    si = 0.0_dp
    sumvalue = 0.0_dp
    Do I = istart+1, iend
       si = si + vec(I)
    End Do
    Call MPI_reduce(si,sumvalue,1,MPI_dp,MPI_Sum,0,comm,ierr)
    Call MPI_Bcast(sumvalue,1,MPI_dp,0,comm,ierr)
  end subroutine Test

End Module testsum

Program KindReduce
  use mpi_f08
  use testsum
  implicit none
  integer,parameter :: vecsize=500
  integer :: i
  real(kind=dp) :: vec(vecsize) = [ (1,i=1,vecsize) ]
  real(kind=dp) :: sumvalue

  call MPI_init()
  call Test( MPI_COMM_WORLD,vec,sumvalue )
  print *,sumvalue
  call MPI_Finalize()

End Program KindReduce
