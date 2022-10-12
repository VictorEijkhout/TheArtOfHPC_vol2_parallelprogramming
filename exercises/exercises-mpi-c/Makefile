# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-2021
#### eijkhout@tacc.utexas.edu
####
#### Makefile for MPI course exercises in C
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
    commrank prime randommax onenorm sumsquares \
    pingpong sendrecv scangather transposeblock \
    isendirecv isendirecvarray setdiff \
    bucketblock bucketpipenonblock \
    stridesend cubegather procgrid procgridnonblock \
    randomput countdown onesidedbuild lockall lockfetch lockfetchshared \
    blockwrite blockadvance blockseek blockall viewwrite scatterwrite \
    jordan rightsend rightput rightgraph \
    ibarrierupdate shareddata \
    mcve
SOURCES = $(patsubst %,%.c,${EXERCISES})
OBJS = $(patsubst %,%.o,${EXERCISES})

MODE = mpi
LANGUAGE = C
${OBJS} tools.o : tools.h
EXTRA_OBJS = tools.o
include ../course-support/Make.compile

include ../course-support/Make.simgrid
ifdef TACC_TAU_DIR
include ../course-support/Make.tau
endif

####
#### Regression tests
####

include ../course-support/Make.mpi_regress
CATCHER = ../course-support/catchout
MPIRUN = mpiexec
NP = 4

include ../course-support/Make.clean
# simgrid and regression junk
clean ::
	@/bin/rm -f *.err *.dat *.out[0-9]*
