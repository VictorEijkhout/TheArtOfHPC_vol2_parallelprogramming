################################################################
####
#### This makefile is part of the tutorial
#### `Introduction to the PETSc library'
#### by Victor Eijkhout eijkhout@tacc.utexas.edu
####
#### copyright Victor Eijkhout 2012-2025
####
#### makefile for PETSc course slides
####
################################################################

info::
	@echo "================================================================"
	@echo "makefile for PETSc course slides"
	@echo "================================================================"
	@echo

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


.PHONY: getfiles snippets
getfiles snippets :
	@cd ../.. && make $@

info ::
	@echo "make makefmt"
.PHONY: makefmt
makefmt :
	TEXINPUTS=../..//:${TEXINPUTS} pdftex --ini "&latex" /opt/local/share/texmf-texlive/tex/latex/mylatex/mylatex.ltx preamble.tex
	mv mylatex.fmt mpicourse.fmt


include ../../booksources/makefiles/Make.clean
