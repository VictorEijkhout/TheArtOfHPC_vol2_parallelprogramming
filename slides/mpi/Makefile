################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Programming in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2013-2025
####
#### Makefile for MPI course slides
####
################################################################
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

COURSE = mpi
COURSES  = mpi mpl mpiadvanced mpibasics mpiintermediate \
    test
EXTRATEXINPUTS =

include ../course-support/Make.course
include ../../booksources/makefiles/Make.tex
include ../../booksources/makefiles/Make.texerror

info ::
	@echo "make jordan"
.PHONY: jordan
jordan :
	@TEXINPUTS=../../graphics:../../macros:${TEXINPUTS} ${LATEXENGINE} jordan.tex
clean ::
	@rm -f jordan/*~

exercisecheck :
	exercises=` grep exerciseframe *.tex | grep '\[' | cut -d [ -f 2 | cut -d ] -f 1 \
	            | grep -v serialsend | sort -u ` \
	&& for l in c cxx f f08 p ; do \
	     echo "%%%%%%%%%%%%%%%% $${l} %%%%%%%%%%%%%%%%" \
	     && for e in $${exercises} ; do \
	          h=` ls ../solutions-mpi-$${l}/$${e}* 2>/dev/null \
	              | wc -l ` ; \
	          if [ $${h} -eq 0 ] ; then \
	            echo "Exercise $${e} not present for $${l}" ; \
	          fi ; \
	        done ; \
	   done

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
