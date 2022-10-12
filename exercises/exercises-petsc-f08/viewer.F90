! -*- f90 -*-
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!
!!!! This program file is part of the tutorial
!!!! `Introduction to the PETSc library'
!!!! by Victor Eijkhout eijkhout@tacc.utexas.edu
!!!!
!!!! copyright Victor Eijkhout 2012-8
!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  program mat
#include "petsc/finclude/petsc.h"
    use petsc
    implicit none
    
    PetscViewer fd
    Mat         :: A 
    PetscInt :: ii(1),jj(1)
    PetscScalar :: vv(1)
    IS          perm, iperm
    Vec         u, b
    PetscReal norm
    PetscScalar      v, one,mone
    PetscInt ::         i, j, n 
    PetscBool flag
    PetscErrorCode ierr
    
    call PetscInitialize(PETSC_NULL_CHARACTER,ierr); CHKERRQ(ierr)
    one = 1.0; mone = -1.0
    call PetscOptionsGetInt( &
         PETSC_NULL_OPTIONS,PETSC_NULL_CHARACTER, &
         "-n",n,flag,ierr); CHKERRQ(ierr)
      
    if (flag) then

       call MatCreateSeqAIJ(MPI_COMM_WORLD,n,n,&
            n,PETSC_NULL_INTEGER,A,ierr); CHKERRQ(ierr)
       do i=0 ,n -1
          do j=0 ,n -1
             v = 1./(i+j+1)
             ii(1) = i; jj(1) = j; vv(1) = v
             call MatSetValues(A,1,ii,1,jj,vv,INSERT_VALUES,ierr); CHKERRQ(ierr)
!             call MatSetValue(A,i,j,v,INSERT_VALUES,ierr); CHKERRQ(ierr)
          end do
       end do
       call MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY,ierr); CHKERRQ(ierr)
       call MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY,ierr); CHKERRQ(ierr)
       
       call PetscViewerBinaryOpen&
            (MPI_COMM_WORLD,"matdata",FILE_MODE_WRITE,fd,ierr); CHKERRQ(ierr)
       call MatView(A,fd,ierr); CHKERRQ(ierr)
       call PetscViewerDestroy(fd,ierr); CHKERRQ(ierr)
       
    else

!
! read the matrix
!
       call PetscViewerBinaryOpen&
            (MPI_COMM_WORLD,"matdata",FILE_MODE_READ,fd,ierr); CHKERRQ(ierr)
       call MatCreate(MPI_COMM_WORLD,A,ierr); CHKERRQ(ierr)
       call MatSetType(A,MATSEQAIJ,ierr); CHKERRQ(ierr)
       call MatLoad(A,fd,ierr); CHKERRQ(ierr)
       call PetscViewerDestroy(fd,ierr); CHKERRQ(ierr)
       
       call MatGetSize(A,n,PETSC_NULL_INTEGER,ierr) ; CHKERRQ(ierr)
       print *,"Read a matrix of size",n
       
    end if
    
    call VecCreateSeq(MPI_COMM_WORLD,n,u,ierr); CHKERRQ(ierr)
    call VecSet(u,one,ierr); CHKERRQ(ierr)
    
    call VecDuplicate(u,b,ierr); CHKERRQ(ierr)
    call MatMult(A,u,b,ierr); CHKERRQ(ierr)
    call VecView(b,PETSC_VIEWER_STDOUT_SELF,ierr); CHKERRQ(ierr)
    
    call VecDestroy(u,ierr); CHKERRQ(ierr)
    call VecDestroy(b,ierr); CHKERRQ(ierr)
    call MatDestroy(A,ierr); CHKERRQ(ierr)

    call PetscFinalize(ierr); CHKERRQ(ierr)
    
  end program mat
