# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### Victor Eijkhout
#### eijkhout@tacc.utexas.edu
####
#### Makefile for short course OpenMP templates in Fortran
####
################################################################

info ::

COURSE = omp

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

EXERCISES = parallel pi piadapt taskfactor

MODE = omp
LANGUAGE = F
include ../course-support/Make.compile

##
## CMake support
##
CMAKEFILESDIR = ../course-support
include ../course-support/Make.cmake

include ../course-support/Make.clean
include ../course-support/Make.omp_regress

