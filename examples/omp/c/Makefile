################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2013-2022
####
#### Makefile for OpenMP C example programs
####
################################################################
################################################################

info ::
	@echo "================================================================"
	@echo " make examples for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

info ::
	@echo
	@echo "make app (where app=${APPS})"
APPS = hello hellocount version proccount pi speedup scaling \
    parfor critfun reductexpr reductpar taskreduct \
    private alloc sharing race taskfor \
    scan scanintsum \
    threadprivate ireduct ireductinit ureduct reductpositive reductarray \
    taskgroup0 taskgroup1 taskgroup2 taskgroup3 taskgroup4 taskgroup5

MODE = omp
LANGUAGE = C
include ../../../makefiles/Make.compile

include  ../../../makefiles/Make.omp_run
RUNS += run_version
run_version : version
	@make --no-print-directory omp_run PROGRAM=version

RUNS += run_parfor
run_parfor : parfor
	@make --no-print-directory omp_run PROGRAM=parfor

RUNS += run_speedup
run_speedup : speedup
	@make --no-print-directory omp_scaling PROGRAM=speedup

RUNS += run_privarray
run_privarray : alloc
	@make --no-print-directory omp_run PROGRAM=alloc NP=4 ECHO=${ECHO} \
	| awk '/private/ {p=0} p==1 {print} /Private/ {p=1} \
	    /Executing/ {print}'

RUNS += run_pointarray
run_pointarray : alloc
	@make --no-print-directory omp_run PROGRAM=alloc NP=4 \
	| awk '/pointer/ {p=0} p==1 {print} /Pointer/ {p=1} \
	    /Executing/ {print}'

RUNS += run_private
run_private : private
	@make --no-print-directory omp_run PROGRAM=private NP=4

RUNS += run_scaling
run_scaling : scaling
	@make --no-print-directory omp_run PROGRAM=scaling

RUNS += run_scan
run_scan : scan
	@make --no-print-directory omp_run PROGRAM=scan

RUNS += run_scansum
run_scansum : scanintsum
	@make --no-print-directory omp_run PROGRAM=$<

RUNS += run_race
run_race : race
	@( \
	for t in ${OMPTHREADS} ; do \
	    echo "On $${t} threads:" \
	    && OMP_NUM_THREADS=$${t} ./race \
	    ; \
	done \
	) 2>&1 | tee racecounter.out

RUNS += run_pi
run_pi : pi
	@export OMP_PROC_BIND=true && export OMP_PLACES=cores \
	 && for n in 100 10000 100000 1000000 10000000 ; do \
	        echo "================ N=$$n" \
	         && for t in ${OMPTHREADS} ; do \
	                OMP_NUM_THREADS=$${t} ./pi $$n \
	                ; \
	            done \
	        ; \
	    done

RUNS += run_proccount
run_proccount : proccount
	for t in 1 2 4 8 16 ; do \
	    echo "================ env: $$t threads" \
	     && OMP_NUM_THREADS=$$t ./proccount \
	    ; done

.PHONY: ${RUNS}

PUBLICEXTS = c runout
include ../../../makefiles/Make.public

include ../../../makefiles/Make.clean
