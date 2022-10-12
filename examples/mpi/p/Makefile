################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2022
####
#### This is the makefile for the python examples
####     in examples/mpi/p
####
################################################################
################################################################

info ::
	@echo "================================================================"
	@echo " make examples for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

info ::
	@echo "make regression"
TESTS = \
    allreduce.py anysource.py bcast.py bcastcolumn.py \
    buffertoolarge.py buffer4g.py \
    gatherv.py getfence.py  irecvsource.py \
    hello.py irecvloop.py irecvsingle.py persist.py putfence.py \
    pingpong.py pingpongbig.py scan.py scipingpong.py tags.py \
    randomupdatefence.py \
    commdup.py commsplit.py
APPS = \
    allreduce anysource bcast buffertoolarge inttype \
    gatherv getfence \
    hello irecvsingle irecvloop irecvsource persist putfence rowcol \
    pingpong scan scipingpong pingpongbig \
    commdup commsplit 
WRONGS = pingpongscalar.py buffer4g 

RUNS = 
LANGUAGE = python
PYTHON = python3
PROGRAM_EXTENSION = .py
include ../../../makefiles/Make.mpi_run

info ::
	@echo "make run_spawn : process spawning example"
RUNS += run_spawn
.PHONY: run_spawn
run_spawn :
	@make regression APPS=spawn_manager

RUNS += run_dup
.PHONY: run_dup
run_dup :
	@make --no-print-directory mpirun PROGRAM=commsplit NP=8

RUNS += run_buffer
.PHONY: run_buffer
run_buffer :
	@make --no-print-directory mpirun PROGRAM=bcastcolumn NP=6

RUNS += run_version
.PHONY: run_version
run_version :
	@make --no-print-directory mpirun PROGRAM=version.py NP=1

TESTSWRONG = buffertoosmall.py 
TESTSNOT = exscan.py
TESTS_IN_PROGRESS =  interleave.py
#include ../../../makefiles/Make.mpi_run
PYTHON = python3
regression :
	@for p in ${APPS} ; do \
	    echo "Testing $$p" \
	    && if [ $${p} = "spawn_manager" ] ; then \
	        export ENVOPTIONS="MY_MPIRUN_OPTIONS=\"-usize 8\"" ; \
	       fi \
	    && make run PROGRAM=$${p}.py ENVOPTIONS="$${ENVOPTIONS}" \
	    ; \
	done

info ::
	@echo "make listsources"
SOURCES = mpi.py hello.py pingpong.py scipingpong.py bcast.py buffertoosmall.py buffertoolarge.py \
  gatherv.py allreduce.py getfence.py
listsources :
	@echo ${SOURCES}

include  ../../../makefiles/Make.clean
