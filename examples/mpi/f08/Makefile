################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2022
####
#### This is the makefile for the examples/mpi/f08
####
################################################################
################################################################

info ::
	@echo "================================================================"
	@echo " make examples for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

APPS = f08 procname count reduceinplace reduceinplaceptr section sectionisend \
    anysource putfence matchkind contiguous struct tags \
    waitnull vector vectorx pingpongbig
WRONGS = typecheck typecheckarg typecheckbuf typecheck8

LANGUAGE = F
MODE = mpi

info ::
	@echo "make putfence0"
.PHONY: putfence0
putfence0 :
	@make putfence0.o EXTRA_FLAGS=-DZERO

include ../../../makefiles/Make.compile

app_dependencies =
.SECONDEXPANSION:

RUNS =
include ../../../makefiles/Make.mpi_run

RUNS += run_waitnull
.PHONY: run_waitnull
run_waitnull : waitnull
	${MPIEXEC} ${NP} waitnull

RUNS += run_count
.PHONY: run_count
run_count : count
	@make --no-print-directory run_generic PROGRAM=count

RUNS += run_procname
.PHONY: run_procname
run_procname : procname
	@make --no-print-directory run_generic PROGRAM=procname

include  ../../../makefiles/Make.clean
