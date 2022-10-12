# -*- makefile
################################################################
####
#### This makefile is part of the tutorial
#### `Introduction to the PETSc library'
#### by Victor Eijkhout eijkhout@tacc.utexas.edu
####
#### copyright Victor Eijkhout 2012-2022
####
#### Makefile for Python exercises
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
EXERCISES = hello vec matvec ksp grid
#matvec grid
#grid mat ksp shell ex46 ex46a
np_root = 1
np_oddeven = 2
np_viewer = 1
LANGUAGE = python
MODE = petsc
include ../course-support/Make.compile
PROGRAMEXT = .py
include ../course-support/Make.mpi_run

# no need to make the examples, so:
.PHONY: ${EXERCISES}

.SECONDEXPANSION:

info ::
	@echo "make run [PROGRAM=prog (default=${PROGRAM})]"
	@echo "         [NP=nn (default=${NP})]"
	@echo "         [PYTHON=... (default=${PYTHON})]"
	@echo "         [OPTIONS=programoptions]"
NP = 4
PYTHON = 

##skip
info ::
	@echo "make regression"
.PHONY: regression
regression :
	@if [ ! -z "${PYTHON}" ] ; then \
	    export mypython=${PYTHON} ; \
	 else \
	    export mypython=python3 ; \
	 fi \
	 && for p in ${EXERCISES} ; do \
	      echo ".. running $${p}.py" ; \
	      mpirun -np 4 $$mypython $${p}.py ${OPTIONS} ; \
	    done
##endskip

include ../course-support/Make.clean
clean ::
	@/bin/rm -f ${EXERCISES}

