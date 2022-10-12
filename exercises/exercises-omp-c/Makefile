# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2017-2020
#### eijkhout@tacc.utexas.edu
####
#### Makefile for short course OpenMP templates in C
####
################################################################

info ::

COURSE = omp

info ::
	@echo "make EXERCISE (where EXERCISE=${EXERCISES})"
EXERCISES = parallel pi piadapt taskfactor

MODE = omp
LANGUAGE = C
include ../course-support/Make.compile
EXTRA_LIBS = -lm

####
#### Regression tests
####

include ../course-support/Make.omp_regress
include ../course-support/Make.clean
