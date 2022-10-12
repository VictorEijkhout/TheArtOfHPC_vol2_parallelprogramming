# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-2021
#### eijkhout@tacc.utexas.edu
####
#### Makefile for MPI course exercises in C++
####
################################################################

info ::

COURSE = mpi

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

EXERCISES = hello \
    commrank prime randommax jordan sumsquares onenorm \
    pingpong sendrecv stridesend transposeblock scangather \
    rightsend bucketblock bucketpipenonblock setdiff \
    isendirecv isendirecvarray procgrid procgridnonblock \
    rightput randomput onesidedbuild countdown lockfetch \
    blockwrite viewwrite scatterwrite cubegather \
    ibarrierupdate rightgraph shareddata
info ::
	@echo "    [ OPTS=... ] (for instance: OPTS=\"-DN 100\")"
	@echo "    support options:"
	@echo "      bucketblock : N"

MODE = mpi
LANGUAGE = CXX
EXTRA_OBJS = tools.o
${OBJS} tools.o : tools.h
include ../course-support/Make.compile


####
#### Regression tests
####

include ../course-support/Make.mpi_run
include ../course-support/Make.mpi_regress

include ../course-support/Make.clean
