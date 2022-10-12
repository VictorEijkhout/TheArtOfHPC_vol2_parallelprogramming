# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2018-2020
#### eijkhout@tacc.utexas.edu
####
#### Makefile for MPI course exercises with simgrid
####
################################################################

info ::

COURSE = mpi
MODE = mpi

info ::
	@echo "make EXERCISE (where EXERCISE=${EXERCISES})"
EXERCISES = \
    bcast1 bcast2 doubling1 doubling2 \
    rightsend rightput
PROGRAM = bcast1

include ../course-support/Make.course
include ../course-support/Make.simgrid
include ../../makefiles/Make.compile
include ../../makefiles/Make.clean

clean ::
	@/bin/rm -f ${EXERCISES}
