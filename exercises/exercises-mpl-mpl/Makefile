# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-2026
#### eijkhout@tacc.utexas.edu
####
#### Makefile for MPL course exercises
####
################################################################

info ::

COURSE = mpi

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

EXERCISES = commrank reduce gather pingpong

SOURCES = $(patsubst %,%.cpp,${EXERCISES})
OBJS = $(patsubst %,%.o,${EXERCISES})

info ::
	@echo "    [ OPTS=... ] (for instance: OPTS=\"-DN 100\")"
	@echo "    support options:"
	@echo "      bucketblock : N"

MODE = mpl
LANGUAGE = CXX
EXTRA_OBJS = tools.o
include ../course-support/Make.compile
CPPSTANDARD = 23
EXTRA_OPTIONS = -I${TACC_MPL_INC}
EXTRA_LIBS = -lstdc++
${OBJS} tools.o : tools.h

include ../course-support/Make.simgrid
ifdef TACC_TAU_DIR
include ../course-support/Make.tau
endif

##
## CMake support
##
CMAKEFILESDIR = ../course-support
include ../course-support/Make.cmake

RUNS =
include ../course-support/Make.mpi_run

RUNS += run_commrank
run_commrank : commrank
	@make mpirun PROGRAM=$< ECHO=${ECHO} CATCHER=

####
#### Regression tests
####
## include ../course-support/Make.mpi_regress

CATCHER = ../course-support/catchout
MPIRUN = mpiexec
NP = 4

include ../course-support/Make.clean
# simgrid junk
clean ::
	@/bin/rm -f *.err *.dat
