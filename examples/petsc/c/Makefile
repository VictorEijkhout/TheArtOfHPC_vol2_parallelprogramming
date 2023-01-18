################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2022
####
#### This is the makefile for the C examples
####     in examples/petsc/c
####
################################################################
################################################################

MAKEFLAGS += --no-builtin-rules

info ::
	@echo "================================================================"
	@echo " make examples for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

APPS = init backtrace sizes split vecarray \
    optionsbegin  \
    average fftsine oddeven power transpose\
    dmrhs grid1d grid2d quintic1 \
    kspcg ksplu twoksps direct
WRONGS = central splitinner

include ${PETSC_DIR}/lib/petsc/conf/variables
MODE = petsc
LANGUAGE = C
OPTLEVEL=2
CORESPERNODE = 16
EXTRA_OPTIONS = -DCORES_PER_NODE=${CORESPERNODE}
include ../../../makefiles/Make.compile
info ::
	@echo "extra compile options:"
	@echo "    [ CORESPERNODE=nnn (default: ${CORESPERNODE})]"
	@echo "    [ EXTRAFLAGS=whatever]"
	@echo "      supported: EXTRAFLAGS=-DDEBUG"

RUNS = 
include ../../../makefiles/Make.mpi_run

RUNS += run_sizes
.PHONY: run_sizes
run_sizes : sizes

RUNS += run_dmcreate
.PHONY: run_dmcreate
run_dmcreate : dmrhs
	@mpiexec -n 4 dmrhs \
	| awk '/create/ {p=0} p==1 {print} /Create/ {p=1}'

include  ../../../makefiles/Make.clean

clean ::
	@rm -f *.dat
