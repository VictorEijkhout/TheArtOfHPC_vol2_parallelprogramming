# -*- makefile -*-
################################################################
################################################################
####
#### This is the makefile for the book
#### `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2012-2021
####
################################################################
################################################################

info ::
	@echo "Do: make pdf"
	@echo "to format the book"

LATEXENGINE = xelatex -shell-escape
BOOKNAME = EijkhoutParallelProgramming

onepass :
	TEXINPUTS=.:../examples//:graphics//:mpireference:snippets:macros:graphics:hwloc:..//:${TEXINPUTS} \
	    ${LATEXENGINE} ${BOOKNAME}
pdf :
	make --no-print-directory onepass
	make --no-print-directory exercises_extract
	make --no-print-directory onepass
	make --no-print-directory onepass
	cp ${BOOKNAME}.pdf ../
.PHONY: diagnostics
info ::
	@echo "make diagnostics : find LaTeX reference problems"
diagnostics :
	@echo "================ Diagnostics ================" \
	 && grep multiply ${BOOKNAME}.log | grep -v header \
	 && echo "----------------" \
	 && grep undefined ${BOOKNAME}.log | grep -v def: | grep -v although
exercises_extract : 
	@echo "Extracting exercises"
	@cd exercises ; \
	for e in *.tex ; do \
	  if [ `cat $$e | grep label | wc -l` -eq 1 ] ; then \
	    export nam=`cat $$e | grep label | sed -e s/^.*label.// -e s/\}//` ; \
	    cp $$e $$nam.tex ; \
	  fi ; \
	done
clean ::
	@rm -f ${BOOKNAME}.pdf acronyms.tex chapternames.tex

info ::
	@echo "Do: make clean"
	@echo "to clean up temporary files"
clean ::
	@/bin/rm -f *.{aux,blg,idx,ilg,ind,log,out,toc} *~ comment.cut \
	 exercises/*.tex snippets/*.cut

