# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2017-2020
#### eijkhout@tacc.utexas.edu
####
#### Makefile for short course OpenMP templates in C++
####
################################################################

info ::

COURSE = omp

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

EXERCISES = mapreduce

MODE = omp
LANGUAGE = CXX
include ../course-support/Make.compile
EXTRA_LIBS = 

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
