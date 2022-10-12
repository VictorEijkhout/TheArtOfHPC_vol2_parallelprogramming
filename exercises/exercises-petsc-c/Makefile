# -*- makefile
################################################################
####
#### This makefile is part of the tutorial
#### `Introduction to the PETSc library'
#### by Victor Eijkhout eijkhout@tacc.utexas.edu
####
#### copyright Victor Eijkhout 2012-2022
####
#### Makefile for petsc C exercises
####
################################################################

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

include ${PETSC_DIR}/lib/petsc/conf/variables
LOCDIR = csolutions

EXERCISE = hello
EXERCISES = hello root vec matvec ksp shell oddeven grid plex
#grid mat ksp shell ex46 ex46a
np_root = 1
np_oddeven = 2
np_viewer = 1
LANGUAGE = C
MODE = petsc
include ../course-support/Make.compile
include ../course-support/Make.mpi_run

##skip
info ::
	@echo "make regression"
.PHONY: regression
regression :
	@for c in ${SOURCES} ; do \
	  echo ".. compiling $$c" ; \
	  make $$c >/dev/null 2>&1 ; \
	done
	@for c in ${SOURCES} ; do \
	  echo ".. running $$c" ; \
	  mpirun -np 4 $$c ; \
	done
##endskip

include ../course-support/Make.clean
clean ::
	@rm -f mat.data*
