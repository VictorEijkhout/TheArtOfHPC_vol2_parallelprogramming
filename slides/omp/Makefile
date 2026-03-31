################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Programming in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2013-2026
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
LECTURE = forkjoin
LECTURES = forkjoin parloop workshare ompdata sync tasks affinity memory other
LONGNAME_forkjoin = 07 Fork-Join model
LONGNAME_parloop = 08 Parallel Loops
LONGNAME_workshare = 09 Worksharing
LONGNAME_ompdata = 10 Data Model
LONGNAME_sync = 11 Synchronization
LONGNAME_performance = 12 OpenMP Performance
LONGNAME_tasks = 13 Tasks
LONGNAME_affinity = 14 Affinity
LONGNAME_memory = 15 Memory

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
