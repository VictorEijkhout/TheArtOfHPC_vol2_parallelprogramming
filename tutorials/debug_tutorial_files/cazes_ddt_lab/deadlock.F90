! Example program that purposefully deadlocks MPI communication by using
! blocking communication statements in the wrong order. From an original
! example by John Cazes @ TACC
!
! Compile using: mpif90 -g -O0 -o deadlock deadlock.f90
!
! v1.0 - 2009/03/12 - carlos@tacc.utexas.edu

PROGRAM Deadlock
USE MPI

! Local variables
INTEGER :: i, proc, nprocs, MPI_ERR
INTEGER :: MPI_STAT(MPI_STATUS_SIZE)
INTEGER, PARAMETER :: NMAX = 25000, TAG1 = 1, TAG2 = 2
INTEGER :: snd_buffer(1:NMAX), rcv_buffer(1:NMAX)

! Initialize MPI communication
! Total number of processors is nprocs
! Processor ranks range from 0 to (nprocs-1)
CALL MPI_INIT(MPI_ERR)
CALL MPI_COMM_SIZE(MPI_COMM_WORLD, nprocs, MPI_ERR)
CALL MPI_COMM_RANK(MPI_COMM_WORLD, proc, MPI_ERR)

! Initialize data
DO i = 1, NMAX

  IF ( proc == 0 ) THEN
    snd_buffer(i) = i
  ELSE
    snd_buffer(i) = 2*i
  END IF
  rcv_buffer(i) = 0

END DO

! Write some data to verify the data exchange later on
IF ( proc == 0 )        WRITE(6,*) 'P:',proc,snd_buffer(1),snd_buffer(NMAX)
IF ( proc == nprocs-1 ) WRITE(6,*) 'P:',proc,snd_buffer(1),snd_buffer(NMAX)

! Exchange data
DO i = 1, nprocs-1

! Send information from processor 0 to processor i and collect information from
! processor i into processor 0 using blocking comms.
  IF ( proc == 0 ) THEN
    CALL MPI_SEND(snd_buffer, NMAX, MPI_INTEGER, i, TAG1, &
                  MPI_COMM_WORLD, MPI_ERR)

    CALL MPI_RECV(rcv_buffer, NMAX, MPI_INTEGER, i, TAG2, &
                  MPI_COMM_WORLD, MPI_STAT, MPI_ERR)
  END IF

! Send information from processor i to processor 0 and collect information from
! processor 0 into processor i using blocking comms.
  IF ( proc == i ) THEN
    CALL MPI_SEND(snd_buffer, NMAX, MPI_INTEGER, 0, TAG2, &
                  MPI_COMM_WORLD, MPI_ERR)

    CALL MPI_RECV(rcv_buffer, NMAX, MPI_INTEGER, 0, TAG1, &
                  MPI_COMM_WORLD, MPI_STAT, MPI_ERR)
  END IF

END DO

! Make sure all processes are synchronized
CALL MPI_BARRIER(MPI_COMM_WORLD, MPI_ERR)

! Write some data to verify the data exchange
IF ( proc == 0 )        WRITE(6,*) 'P:',proc,rcv_buffer(1),rcv_buffer(NMAX)
IF ( proc == nprocs-1 ) WRITE(6,*) 'P:',proc,rcv_buffer(1),rcv_buffer(NMAX)

! Finilize MPI and stop program
CALL MPI_FINALIZE(MPI_ERR)

END PROGRAM
