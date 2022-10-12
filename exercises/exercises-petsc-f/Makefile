# -*- makefile
################################################################
####
#### This makefile is part of the tutorial
#### `Introduction to the PETSc library'
#### by Victor Eijkhout eijkhout@tacc.utexas.edu
####
#### copyright Victor Eijkhout 2012-2022
####
#### Makefile for Fortran90 exercises
####
################################################################

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

include ${PETSC_DIR}/lib/petsc/conf/variables
LOCDIR = fsolutions

EXERCISE = hello
EXERCISES = hello root vec matvec ksp oddeven grid
np_root = 1
np_oddeven = 2
np_viewer = 1
LANGUAGE = F
MODE = petsc
include ../course-support/Make.compile
include ../course-support/Make.mpi_run

shell : fivepoint.o

##skip
info ::
	@echo "make regression"
.PHONY: regression
regression :
	@for c in ${EXERCISES} ; do \
	  echo ".. compiling $$c" ; \
	  make $$c >/dev/null 2>&1 ; \
	done
	@for c in ${EXERCISES} ; do \
	  echo ".. running $$c" ; \
	  mpirun -np 4 $$c ; \
	done
##endskip

include ../course-support/Make.clean
clean ::
	@rm -f mat.data*
