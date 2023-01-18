################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2020
####
#### This is the makefile for the C examples
####     in examples/petsc/p
####
################################################################
################################################################

MAKEFLAGS += --no-builtin-rules

info ::
	@echo "================================================================"
	@echo " make examples for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

APPS = setvalues hdf5

include ${PETSC_DIR}/lib/petsc/conf/variables
MODE = petsc
LANGUAGE = P
OPTLEVEL=2
CORESPERNODE = 16
EXTRA_OPTIONS = -DCORES_PER_NODE=${CORESPERNODE}
include ../../../makefiles/Make.compile
info ::
	@echo "extra compile options:"
	@echo "    [ CORESPERNODE=nnn (default: ${CORESPERNODE})]"
	@echo "    [ EXTRAFLAGS=whatever]"
	@echo "      supported: EXTRAFLAGS=-DDEBUG"

.SECONDEXPANSION:

include  ../../../makefiles/Make.clean

RUNS = 
include ../../../makefiles/Make.run

clean ::
	@rm -f *.dat 
