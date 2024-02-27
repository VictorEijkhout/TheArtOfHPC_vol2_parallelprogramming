# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2017-2024
#### eijkhout@tacc.utexas.edu
####
#### Makefile for short course OpenMP templates in C
####
################################################################

info ::

COURSE = omp

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

EXERCISES = parallel pi piadapt jacobi taskfactor vectorsum

MODE = omp
LANGUAGE = C
include ../course-support/Make.compile
EXTRA_LIBS = -lm

##
## CMake support
##
CMAKEFILESDIR = ../course-support
include ../course-support/Make.cmake

####
#### Regression tests
####

include ../course-support/Make.omp_regress
include ../course-support/Make.clean
