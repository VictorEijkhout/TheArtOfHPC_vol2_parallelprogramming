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
APPS = hello hellocount compliant proccount pi speedup scaling \
    parfor critfun reductexpr reductpar taskreduct \
    private alloc sharing race taskfor scan \
    threadprivate ireduct ireductinit ureduct reductpositive reductarray \
    taskgroup0 taskgroup1 taskgroup2 taskgroup3 taskgroup4 taskgroup5

MODE = omp
LANGUAGE = C
include ../../../makefiles/Make.compile

include  ../../../makefiles/Make.omp_run
RUNS += run_parfor
.PHONY: run_parfor
run_parfor : parfor
	@make --no-print-directory omp_run PROGRAM=parfor

RUNS += run_speedup
.PHONY: run_speedup
run_speedup : speedup
	@make --no-print-directory omp_scaling PROGRAM=speedup

RUNS += run_private
.PHONY: run_private
run_private : private
	@make --no-print-directory omp_run PROGRAM=private NP=4

RUNS += run_scaling
.PHONY: run_scaling
run_scaling : scaling
	@make --no-print-directory omp_run PROGRAM=scaling

RUNS += run_scan
.PHONY: run_scan
run_scan : scan
	@make --no-print-directory omp_run PROGRAM=scan

.PHONY: run_race
RUNS += run_race
run_race : race
	( \
	for t in ${OMPTHREADS} ; do \
	    echo "On $${t} threads:" \
	    && OMP_NUM_THREADS=$${t} ./race \
	    ; \
	done \
	) 2>&1 | tee racecounter.out

.PHONY: run_pi
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
.PHONY: run_proccount
run_proccount : proccount
	for t in 1 2 4 8 16 ; do \
	    echo "================ env: $$t threads" \
	     && OMP_NUM_THREADS=$$t ./proccount \
	    ; done

include ../../../makefiles/Make.clean
