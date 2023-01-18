################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2020
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

APPS = init initfile mainhelp backtrace oddeven vecset

##
## Compilation
##
ifdef PETSC_DIR
include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules
endif
LANGUAGE = F
MODE = petsc
include ../../../makefiles/Make.compile
CFLAGS = ${PETSC_FC_INCLUDES}

# OPTLEVEL=2
# CORESPERNODE = 16
# EXTRA_OPTIONS = -DCORES_PER_NODE=${CORESPERNODE}

##
## Runs
##
RUNS =
RUNS += run_help
run_help : mainhelp
	@make --no-print-directory mpirun PROGRAM=mainhelp \
	        OPTIONS="-help" CATCHER= NP=1 \
	    2>&1 | grep -v TACC | head -n 1
.PHONY: ${RUNS}
include ../../../makefiles/Make.mpi_run


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

include  ../../../makefiles/Make.clean

clean ::
	@rm -f *.dat
