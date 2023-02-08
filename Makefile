# -*- makefile -*-
################################################################
################################################################
####
#### This is the makefile for the book
#### `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2012-2023
####
#### public repo top level makefile
####
################################################################
################################################################

info ::
	@echo \
	&& echo "Top level makefile for the public repo of" \
	&& echo "  Parallel Programming for Science and Engineering " \
	&& echo "  Victor Eijkhout" \
	&& echo \
	&& echo "Available rules:"

.PHONY: pdf slides
info ::
	@echo "make pdf : compile the book"
	@echo "make slides : compile courses ${COURSES}"
COURSES = mpi omp
pdf :
	@cd booksources && make --no-print-directory pdf
slides :
	@for c in ${COURSES} ; do \
	    ( cd slides/$$c && make --no-print-directory course ) \
	; done

.PHONY: clean
info ::
	@echo "make clean"
clean ::
	@rm -f makefiles/*~
	@for d in booksources examples exercises slides ; do \
	    ( echo "cleaning in <<$$d>>" && cd "$$d" && make clean ) \
	    ; done

