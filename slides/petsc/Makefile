################################################################
####
#### This makefile is part of the tutorial
#### `Introduction to the PETSc library'
#### by Victor Eijkhout eijkhout@tacc.utexas.edu
####
#### copyright Victor Eijkhout 2012-2024
####
#### makefile for PETSc course slides
####
################################################################

info::
	@echo "================================================================"
	@echo "makefile for MPI course slides"
	@echo "================================================================"
	@echo

info ::
	@echo "make makefmt"
.PHONY: makefmt
makefmt :
	TEXINPUTS=../..//:${TEXINPUTS} pdftex --ini "&latex" /opt/local/share/texmf-texlive/tex/latex/mylatex/mylatex.ltx preamble.tex
	mv mylatex.fmt mpicourse.fmt

COURSE = petsc
COURSES = petsc
FILE = petsc_course
FILES = petsc_course petsc-handout

EXTRATEXINPUTS=:../../petscreference
LANGUAGE = all
LANGUAGES = f p c
include ../course-support/Make.course
include ../../booksources/makefiles/Make.tex
include ../../booksources/makefiles/Make.texerror

include ../../booksources/makefiles/repos.inc

include ../../booksources/makefiles/Make.clean
