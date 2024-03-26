################################################################
################################################################
####
#### This Makefile is part of the textbook
#### "Parallel computing for science and engineering"
#### by Victor Eijkhout
#### copyright 2013-2024
####
#### Makefile for debugging examples in C, parallel
####
################################################################
################################################################

info ::

COURSE = ddt
info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"
	@echo
	@echo "make <progname> (in: ${EXERCISES})"
	@echo "    sequential : ${SEQUENTIAL}"
	@echo "    parallel   : ${PARALLEL}"

EXERCISES = bcast reduce finalize sendrecv1 sendrecv2 isendrecv ring ring_1a

MODE = mpi
LANGUAGE = C
include ../course-support/Make.compile
OPTLEVEL = 1
.SECONDEXPANSION:

ifdef TACC_TAU_DIR
include ../course-support/Make.tau
endif

include ../course-support/Make.clean

