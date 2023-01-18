################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2022
####
#### This is the makefile for the F90 examples
####     in examples/petsc/f
####
################################################################
################################################################

MAKEFLAGS += --no-builtin-rules

info ::
	@echo "================================================================"
	@echo " make examples for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"
	@echo "Do: make app (where app=${APPS})"

APPS = backtrace vecarray

include ${PETSC_DIR}/lib/petsc/conf/variables
MODE = petsc
LANGUAGE = F
OPTLEVEL=2
CORESPERNODE = 16
EXTRA_OPTIONS = -DCORES_PER_NODE=${CORESPERNODE}
include ../../../makefiles/Make.compile
info ::
	@echo "extra compile options:"
	@echo "    [ CORESPERNODE=nnn (default: ${CORESPERNODE})]"
	@echo "    [ EXTRAFLAGS=whatever]"
	@echo "      supported: EXTRAFLAGS=-DDEBUG"

.PHONY: run_initfile
info ::
	@echo "make run PROGRAM (in: ${APPS})"
PROGRAM = init
NP = 4
RUN_OPTIONS = 
.PHONY: run
run : ${PROGRAM}
	@if [ ${PROGRAM} = "initfile" ] ; then \
	    echo "-help \"Init Example\"" > inithelp \
	    && echo "Options file:" && cat inithelp \
	    ; fi
	mpiexec -n ${NP} ./${PROGRAM} ${RUN_OPTIONS}

RUNS = 
include ../../../makefiles/Make.run

include  ../../../makefiles/Make.clean

clean ::
	@rm -f *.dat
