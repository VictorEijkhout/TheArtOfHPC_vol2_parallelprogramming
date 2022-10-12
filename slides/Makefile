# -*- makefile -*-
################################################################
################################################################
####
#### This is the makefile for the book
#### `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2012-2021
####
#### public repo slides directory makefile
####
################################################################
################################################################

info ::
	@echo \
	&& echo "Makefile for the lecture slides directory" \
	&& echo \
	&& echo "Go into the subdirectories ${COURSES} to make slides" \
	&& echo && echo "or:"

COURSES =  ddt mpi omp petsc

.PHONY: clean
info ::
	@echo "make clean"
clean ::
	@for d in ${COURSES} ; do \
	    ( echo ".. cleaning in <<$$d>>" && cd "$$d" && make clean ) \
	    ; done

