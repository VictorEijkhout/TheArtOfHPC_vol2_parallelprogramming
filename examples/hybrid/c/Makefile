################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2022
####
#### This is the makefile for the C examples
####     in examples/mpi/c
####
################################################################
################################################################

info ::
	@echo "================================================================"
	@echo " make examples for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

APPS = thread procthread portappmultiple portpthread anytag

MODE = hybrid
LANGUAGE = C
include ../../../makefiles/Make.compile

RUNS += run_anytag
run_anytag : anytag
	@make --no-print-directory mpirun PROGRAM=$< \
	    NP=2 ENVOPTIONS="OMP_NUM_THREADS=2" ECHO=${ECHO}

RUNS += run_portthread
run_procthread : procthread
	@if [ ${SLURM_NNODES} -ne 1 ] ; then \
	    echo "Run this only on one node" && exit 1 \
	    ; \
	fi
	@echo "==== NNODES: ${SLURM_NNODES}"
	@for np in `seq 1 ${SLURM_NPROCS}` ; do \
	    echo "---- nprocs: $${np}" \
	     && OMP_NUM_THREADS=17 mpiexec -n $${np} procthread \
	    ; \
	done

RUNS += run_portmultiple run_portpthread
run_portmultiple : portappmultiple
run_portpthread : portpthread
run_portmultiple run_portpthread : 
	@export FI_MLX_ENABLE_SPAWN=yes \
	 && export FI_MLX_NS_ENABLE=1 \
	 && make --no-print-directory mpirun PROGRAM=./$^ ECHO=${ECHO}

.PHONY: ${RUNS}
include  ../../../makefiles/Make.mpi_run

include  ../../../makefiles/Make.clean
