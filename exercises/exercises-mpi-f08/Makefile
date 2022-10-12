# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-2022
#### eijkhout@tacc.utexas.edu
####
#### Makefile for  MPI course exercises in Fortran2008
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
    rightsend rightput rightgraph \
    bucketblock bucketpipenonblock setdiff \
    isendirecv isendirecvarray status \
    stridesend cubegather onenorm procgrid \
    randomput onesidedbuild countdown lockfetch lockfetchshared \
    blockwrite shareddata ibarrierupdate
OTHERPROGRAMS = write
SOURCES = $(patsubst %,%.F90,${EXERCISES})
OBJS    = $(patsubst %,%.o,${EXERCISES})

MODE = mpi
LANGUAGE = F
EXTRA_OBJS = tools.o
${OBJS} : tools.mod
tools.mod : tools.o
include ../course-support/Make.compile
shareddata : contents.o

####
#### Regression tests
####

include ../course-support/Make.mpi_run
include ../course-support/Make.mpi_regress
run_hello : hello

include ../course-support/Make.clean
