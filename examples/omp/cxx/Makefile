################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2013-2022
####
#### Makefile for OpenMP cxx example programs
####
################################################################
################################################################

info ::
	@echo ; echo "OpenMP example programs" ; echo ; echo "available commands:"

info ::
	@echo
	@echo "make app (where app=${APPS})"
APPS = hello hellocount pi pivector speedup \
    link treesearch taskreduct taskreductwrong \
    reductpositive reductcomplex \
    heat heatalloc heatptr \
    lockobject
WRONGS = reductarray

MODE = omp
LANGUAGE = CXX
include ../../../makefiles/Make.compile
CPPSTANDARD = 17

EXEC = hello
NP = 4
info ::
	@echo
	@echo "make testrun [EXEC=app (where app in ${APPS})]"
testrun:
	make clean; rm -f ${EXEC}; make ${EXEC}
	OMP_NUM_THREADS=${NP} ./${EXEC}

include ../Make.omp

info ::
	@echo "make run_xyz (in: ${RUNS})"
	@echo "    [NP=nnn (default: ${NP})]"
include  ../../../makefiles/Make.omp_run
RUNS =
RUNS += run_speedup
.PHONY: run_speedup
run_speedup : speedup
	@make --no-print-directory omp_scaling PROGRAM=speedup
.PHONY: run_race
RUNS += run_race
run_race : race
	( \
	for t in 1 2 4 8 ; do \
	    echo "On $${t} threads:" \
	    && OMP_NUM_THREADS=$${t} ./race \
	    ; \
	done \
	) 2>&1 | tee ../../../output/racecounter.out
info ::
	@echo "make output : regenerate all output"
.PHONY: output
output :
	@for r in ${RUNS} ; do \
	  make $${r} \
	  ; \
	done

include  ../../../makefiles/Make.clean
