################################################################
################################################################
####
#### This Makefile is part of the textbook
#### "Parallel computing for science and engineering"
#### by Victor Eijkhout
#### copyright 2013-2021
####
#### Makefile for debugging examples in C
####
################################################################
################################################################

info ::

COURSE = ddt
info ::
	@echo "make <progname> (in: ${EXERCISES})"
	@echo "    sequential : ${SEQUENTIAL}"
	@echo "    parallel   : ${PARALLEL}"

PARALLEL = bcast reduce finalize sendrecv1 sendrecv2 isendrecv
SEQUENTIAL = roots array1 array2 array3
EXERCISES = ${PARALLEL} ${SEQUENTIAL}
OTHER_EXERCISES = array0 array1 array2 ring broadcast
.PHONY: sources
sources :
	@echo ${EXERCISES}

MODE = mpi
LANGUAGE = C
include ../course-support/Make.compile
OPTLEVEL = 1
.SECONDEXPANSION:
${SEQUENTIAL} ::
	@echo "making sequential example $@"
	@make --no-print-directory $@ EXERCISES=$@ MODE=omp SEQUENTIAL=none
#  -f Makefile.seq
${PARALLEL} :: $$@.c
	@echo "making parallel example $@"
	@make --no-print-directory $@ EXERCISES=$@ MODE=mpi PARALLEL=none

ifdef TACC_TAU_DIR
include ../course-support/Make.tau
endif

include ../course-support/Make.clean
clean ::
	@/bin/rm -f ${SEQUENTIAL} ${PARALLEL}

