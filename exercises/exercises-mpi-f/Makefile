# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-2021
#### eijkhout@tacc.utexas.edu
####
#### Makefile for  MPI course exercises in Fortran
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
    commrank prime randommax jordan \
    pingpong sendrecv sumsquares scangather \
    rightsend bucketblock bucketpipenonblock lockfetch lockfetchshared \
    isendirecv isendirecvarray setdiff \
    cubegather procgrid \
    rightput randomput countdown onesidedbuild \
    blockwrite shareddata ibarrierupdate
OTHERPROGRAMS = onenorm  write
SOURCES = $(patsubst %,%.F90,${EXERCISES})
OBJS    = $(patsubst %,%.o,${EXERCISES})

MODE = mpi
LANGUAGE = F
EXTRA_OBJS = tools.o
${OBJS} : tools.mod
tools.mod : tools.o
include ../course-support/Make.compile

####
#### Regression tests
####

include ../course-support/Make.mpi_run
include ../course-support/Make.mpi_regress
run_hello : hello

include ../course-support/Make.clean
