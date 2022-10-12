# -*- makefile -*-
################################################################
################################################################
####
#### This is the makefile for the book
#### `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2012-2021
####
#### public repo exercise directory makefile
####
################################################################
################################################################

info ::
	@echo \
	&& echo "Makefile for the exercises directory" \
	&& echo \
	&& echo "Available rules:"

.PHONY: clean
info ::
	@echo "make clean"
clean ::
	@for d in exercises-* ; do \
	    ( echo ".. cleaning in <<$$d>>" && cd "$$d" && make clean ) \
	    ; done

