# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2017-2025
#### eijkhout@tacc.utexas.edu
####
#### Makefile for CUDA exercises
####
################################################################

info ::

COURSE = cuda

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

EXERCISES = hello add1m

MODE = cuda
LANGUAGE = CXX
MODULES = cxxopts
include ../course-support/Make.compile
include ../course-support/Make.cuda
EXTRA_OPTIONS = $$( pkg-config --cflags cxxopts )

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
