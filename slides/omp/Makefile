################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Programming in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2013-2025
####
#### Makefile for OpenMP course slides
####
################################################################
################################################################

info::
	@echo "================================================================"
	@echo "makefile for OpenMP course slides"
	@echo "================================================================"
	@echo

COURSE = omp
COURSES = omp ompcases ompcpp
INEX = tacc-full

include ../course-support/Make.course
include ../../booksources/makefiles/Make.tex
include ../../booksources/makefiles/Make.texerror
.PHONY: getfiles snippets
getfiles snippets :
	@cd ../.. && make $@

include ../../booksources/makefiles/repos.inc

include ../../booksources/makefiles/Make.clean
total_clean ::
	@for f in ${FILES} ; do \
	  rm -f $${f}.pdf ; \
	done
info ::
	@echo "make clean [ PDFEXCLUDE=... ]"
clean ::
	@for f in ${FILES} ; do \
	  if [ "$${f}" != "${PDFEXCLUDE}" ] ; then \
	    /bin/rm -f $${f}.pdf ; \
	  fi ; \
	done
