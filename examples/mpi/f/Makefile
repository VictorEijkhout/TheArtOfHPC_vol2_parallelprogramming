################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2022
####
#### This is the makefile for the examples/mpi/f
####
################################################################
################################################################

info ::
	@echo "================================================================"
	@echo " make examples for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

APPS = init local sendrecv anysource \
    putfence getfence exscan \
    irecvsource reduce reduceinplace reduceinplaceptr allreduce gather gatherv \
    contiguous vector indexed pack \
    persist typematch
WRONGS = sendblock ssendblock

LANGUAGE = F
MODE = mpi
include ../../../makefiles/Make.compile
${patsubst %,%.o,${APPS}} tools.o : globalinit.F90

RUNS = 
include ../../../makefiles/Make.run

include ../../../makefiles/Make.clean
