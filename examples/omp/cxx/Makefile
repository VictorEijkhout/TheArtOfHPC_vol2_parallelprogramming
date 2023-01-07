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
APPS = hello hellocount nested pi pivector speedup vecdata alloc \
    link treesearch taskreduct taskreductwrong \
    reductpositive reductcomplex \
    heat heatalloc heatptr \
    lockobject iterator
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

info ::
	@echo "make run_xyz (in: ${RUNS})"
	@echo "    [NP=nnn (default: ${NP})]"
include  ../../../makefiles/Make.omp_run
RUNS =

RUNS += run_privvector
run_privvector : alloc
	@make --no-print-directory omp_run PROGRAM=alloc NP=4 ECHO=${ECHO} \
	| awk '/private/ {p=0} p==1 {print} /Private/ {p=1} \
	    /Executing/ {print}'

RUNS += run_speedup
run_speedup : speedup
	@make --no-print-directory omp_scaling PROGRAM=speedup

RUNS += run_race
run_race : race
	( \
	for t in 1 2 4 8 ; do \
	    echo "On $${t} threads:" \
	    && OMP_NUM_THREADS=$${t} ./race \
	    ; \
	done \
	) 2>&1 | tee ../../../output/racecounter.out

RUNS += run_nested
run_nested : nested
	@make omp_run PROGRAM=nested NP=2 ACTIVE=2

.PHONY: ${RUNS}

info ::
	@echo "make output : regenerate all output"
.PHONY: output
output :
	@for r in ${RUNS} ; do \
	  make $${r} \
	  ; \
	done

PUBLICEXTS = cxx runout
include ../../../makefiles/Make.public

include  ../../../makefiles/Make.cmake

include  ../../../makefiles/Make.clean
