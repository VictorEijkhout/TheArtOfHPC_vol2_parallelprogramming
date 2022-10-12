################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2020
####
#### Makefile for debugging examples in C
####
################################################################
################################################################

info ::

info ::
	@echo "make <progname> (in: ${PROGS})"
	@echo "    ( sequential : ${SEQUENTIAL})"
	@echo "    ( parallel   : ${PARALLEL})"
	@echo "    [ OPTIONS=... ] "
PARALLEL = bcast reduce finalize sendrecv1 sendrecv2 isendrecv
SEQUENTIAL = roots array1 array2 array3
PROGS = ${PARALLEL} ${SEQUENTIAL}
ifdef TACC_TAU_DIR
  CC = tau_cc.sh -tau_options=-optCompInst
else ifeq "${TACC_FAMILY_COMPILER}" "intel"
  CC = icc
else
  CC = gcc
endif
.SECONDEXPANSION:
${SEQUENTIAL} : $$@.c
	${CC} -g -O0 -o $@ $@.c ${OPTIONS}
${PARALLEL} : $$@.c
	mpicc -g -O0 -o $@ $@.c ${OPTIONS}
info ::
	@echo "make compile: compile all"
compile :
	@for p in ${SEQUENTIAL} ${PARALLEL} ; do \
	  make --no-print-directory $$p ; \
	done

info ::
	@echo "make clean"

clean :
	@/bin/rm -rf ${PROGS} *~ *.o *.dSYM *.r[0-9]* ddt.o*
