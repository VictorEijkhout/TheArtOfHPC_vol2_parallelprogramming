# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### copyright 2015-2025 Victor Eijkhout
#### eijkhout@tacc.utexas.edu
####
#### Makefile for course exercises in Python
####
################################################################

info ::

COURSE = mpi
MODE = mpi
LANGUAGE = p

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

EXERCISES = \
    hello commrank prime randommax pingpong jordan setdiff \
    rightsend isendirecv ibarrierupdate scangather \
    onenorm bucketblock stridesend cubegather transposeblock \
    randomput rightput countdown onesidedbuild \
    lockfetch \
    procgrid blockwrite scatterwrite viewwrite

PYTHON = python3
PROGRAM = hello
OPTIONS =
include ../course-support/Make.compile
PROGRAM_EXTENSION = .py

####
#### Regression tests
####
.PHONY: ${EXERCISES} 

include ../course-support/Make.mpi_run
include ../course-support/Make.mpi_regress
CATCHER = ../course-support/catchout -p ${PYTHON}
include ../course-support/Make.clean
