################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2021
####
#### This is the makefile for the MPL examples
####     in examples/mpi/mpl
####
################################################################
################################################################

MAKEFLAGS += --no-builtin-rules

info ::
	@echo "Do: make app (where app=${APPS})"

APPS = commrank \
    sendscalar  recvstatus sendbuffer sendarray sendrange sendlong sendtemplate \
    isendandirecv irecvsource ireducescalar bufring \
    collectscalar collectbuffer reduceuser \
    contiguous vector struct structscalar indexed indexedblock \
    commcompare commpass commsplit
WRONGS = collectarray sendobject

MODE = mpi
LANGUAGE = CXX
.SECONDEXPANSION:
include ../../../makefiles/Make.compile
CPPSTANDARD = 17
EXTRA_OPTIONS = -I${TACC_MPL_INC}

RUNS =
info ::
	@echo "make run_XXX (in ${RUNS})"
include ../../../makefiles/Make.mpi_run
RUNS += run_commcompare run_rawcompare
run_commcompare run_rawcompare :: commcompare
run_commcompare ::
	@${MPIRUN} -n 1 commcompare
run_rawcompare :: 
	@${MPIRUN} -n 1 commcompare \
	| awk '/rawcompare/ {p=0} p==1 {print} /Rawcompare/ {p=1}'

include ../../../makefiles/Make.clean
#include ../../../teaching/course-support/Make.mpi_regress
CATCHER = ../../../teaching/course-support/catchout

clean ::
	@rm -f *.dat
