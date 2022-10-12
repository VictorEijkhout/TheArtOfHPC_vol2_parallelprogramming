! -*- f90 -*-
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!
!!!! This program file is part of the tutorial
!!!! `Introduction to the PETSc library'
!!!! by Victor Eijkhout eijkhout@tacc.utexas.edu
!!!!
!!!! copyright Victor Eijkhout 2012-2020
!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program Grid2d
#include <petsc/finclude/petsc.h>
#include <petscversion.h>
  use petsc
  implicit none

  MPI_Comm       :: comm
  !! grid variables
  DM             :: grid
  DMDALocalInfo  :: info(20)
  MatStencil     :: row(4,1)
  MatStencil     :: col(4,5)
  PetscScalar    :: val(5)
  PetscInt       :: ncols,iterations
  !! problem variables
  KSP            :: solver
  KSPConvergedReason :: reason 
  Mat            :: A
  Vec            :: x,y, xlocal
  PetscInt       :: nprocs,procno, i,j
  PetscReal      :: one = 1.d0
  PetscErrorCode :: ierr
  character*100  :: textbuf

  call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
  CHKERRA(ierr)
  comm = PETSC_COMM_WORLD
  call MPI_Comm_size(comm,nprocs,ierr)
  if (mod(nprocs,2)/=0) then
     SETERRA(comm,1,"Need even number of processors")
  else
11   format('Running on',x,i3,x,'procs\n')
    write(textbuf,11) nprocs
    call PetscPrintf(comm,textbuf,ierr)
  end if
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Create a 2d grid and a matrix on that grid
  !!
  call DMDACreate2d( comm, &         ! IN arguments:
       DM_BOUNDARY_NONE,DM_BOUNDARY_NONE, &     ! no periodicity and such
       DMDA_STENCIL_STAR, &          ! only connections along the axes
       100,100, &                    ! global size, can be changed with options
       PETSC_DECIDE,PETSC_DECIDE, &  ! processors in each direction
       1, &                          ! degrees of freedom per node
       1, &                          ! stencil width in any direction
       PETSC_NULL_INTEGER,PETSC_NULL_INTEGER, & ! arrays of local sizes
       grid, &                       ! OUT: resulting object
       ierr)
  CHKERRA(ierr)
  call DMSetUp(grid,ierr)
  CHKERRA(ierr)
  call DMCreateMatrix(grid,A,ierr)
  CHKERRA(ierr)

  !!
  !! Print out how the grid is distributed over processors
  !!
  call DMDAGetLocalInfoF90(grid,info,ierr)
  CHKERRA(ierr)
  write(textbuf,10) procno,info(7),info(8),info(7)+info(10),info(8)+info(11)
10 format('[',i3,'] Local part = ',i3,'-',i3,' x ',i3,'-',i3,'\n')
  call PetscSynchronizedPrintf(comm,textbuf,ierr)
  CHKERRA(ierr)
  call PetscSynchronizedFlush(comm,PETSC_NULL_INTEGER,ierr)
  CHKERRA(ierr)

  !!
  !! Fill in the elements of the matrix
  !!
  !! Bonus Exercise:
  !! -- this is code for filling in a 5-point matrix.
  !!    Commented out is code for making it a 9-point matrix. Add this.
  !! -- If you run the augmented code you will get a runtime error.
  !!    Take another look at the DMDACreate2d call and fix one parameter
  !!
  do j=info(8),info(8)+info(11)-1 ! x-first and x-extent
     do i=info(7),info(7)+info(10)-1 ! y-first and y-extent
        row(MatStencil_i,1) = i; row(MatStencil_j,1) = j;
        ncols = 1
        !! diagonal element
        col(MatStencil_i,ncols) = i; col(MatStencil_j,ncols) = j;
        val(ncols) = 4.d0; ! Exercise: This needs to become 8
        ncols = ncols+1

        !! boundary: top row
        if (i>0) then
           col(MatStencil_i,ncols) = i-1; col(MatStencil_j,ncols) = j;
           val(ncols) = -1.d0; ncols = ncols+1
        end if
        if ( .FALSE. .AND. i>0 .and. j>0) then
           col(MatStencil_i,ncols) = i-1; col(MatStencil_j,ncols) = j-1;
           val(ncols) = -1.d0; ncols = ncols+1
        end if
        if ( .FALSE. .AND. i>0 .and. j<info(5)-1) then
           col(MatStencil_i,ncols) = i-1; col(MatStencil_j,ncols) = j+1;
           val(ncols) = -1.d0; ncols = ncols+1
        end if

        !! boundary left and right
        if (j>0) then
           col(MatStencil_i,ncols) = i; col(MatStencil_j,ncols) = j-1;
           val(ncols) = -1.d0; ncols = ncols+1
        end if
        if (j<info(5)-1) then
           col(MatStencil_i,ncols) = i; col(MatStencil_j,ncols) = j+1;
           val(ncols) = -1.d0; ncols = ncols+1
        end if
        
        !! boundary: bottom row
        if (i<info(4)-1) then
           col(MatStencil_i,ncols) = i+1; col(MatStencil_j,ncols) = j;
           val(ncols) = -1.d0; ncols = ncols+1
        end if
        if ( .FALSE. .AND. i<info(4)-1 .and. j>0) then
           col(MatStencil_i,ncols) = i+1; col(MatStencil_j,ncols) = j-1;
           val(ncols) = -1.d0; ncols = ncols+1
        end if
        if ( .FALSE. .AND. i<info(4)-1 .and. j<info(5)-1) then
           col(MatStencil_i,ncols) = i+1; col(MatStencil_j,ncols) = j+1;
           val(ncols) = -1.d0; ncols = ncols+1
        end if

        call MatSetValuesStencil(A,1,row,ncols-1,col,val,INSERT_VALUES,ierr)
        CHKERRA(ierr)
     end do
  end do
  !!
  !! Exercise:
  !! -- there are two lines missing here.
  !!    Run your code and inspect the runtime error.
  !!    Can you figure out what to insert?
  !!
!!!! your code here !!!!

  !!
  !! Create vectors on the grid
  !!
  call DMCreateGlobalVector(grid,x,ierr)
  call VecDuplicate(x,y,ierr)

  !!
  !! Set vector values: first locally, then globally
  !!
  call DMCreateLocalVector(grid,xlocal,ierr)
  call VecSet(xlocal,one,ierr)
  call DMLocalToGlobalBegin(grid,xlocal,INSERT_VALUES,x,ierr)
  call DMLocalToGlobalEnd(grid,xlocal,INSERT_VALUES,x,ierr)
  call VecDestroy(xlocal,ierr)

  !!
  !! Solve a linear system on the grid
  !!
  call KSPCreate(comm,solver,ierr)
  call KSPSetType(solver,KSPBCGS,ierr)
  !!
  !! Exercise:
  !! -- there is a line missing here.
  !!    Run the code and try to deduce from the error message what you missed.
  !!    (Or look elsewhere for inspiration.)
  !!
!!!! your code here !!!!
  call KSPSetFromOptions(solver,ierr)
  call KSPSolve(solver,x,y,ierr)

  !!
  !! Report on success of the solver, or lack thereof
  !!
  call KSPGetConvergedReason(Solver,reason,ierr)
  if (reason<0) then
     write(textbuf,21) reason
21   format("Failure to converge",i3,".\n")
     call PetscPrintf(comm,textbuf,ierr)
     CHKERRA(ierr)
  else
     call KSPGetIterationNumber(Solver,iterations,ierr)
     CHKERRA(ierr)
     write(textbuf,22) iterations
22   format("Converged in",I5," iterations.\n")
     call PetscPrintf(comm,textbuf,ierr)
     CHKERRA(ierr)
  end if

  !!
  !! Destroy all allocated objects
  !!
  call VecDestroy(x,ierr)
  call VecDestroy(y,ierr)
  call MatDestroy(A,ierr)
  call DMDestroy(grid,ierr)
  call PetscFinalize(ierr)

end program Grid2d
