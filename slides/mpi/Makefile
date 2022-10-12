################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Programming in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2013-2022
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

COURSE = mpi
COURSES  = mpi mpl mpithree
EXTRATEXINPUTS =

LANGUAGE = all
LANGUAGES = c f p
course_prep ::
	@cp lang_${LANGUAGE}.tex lang.tex
include ../course-support/Make.course

info ::
	@echo "make jordan.pdf"
.PHONY: jordan.pdf
jordan.pdf :
	@TEXINPUTS=../graphics:${TEXINPUTS} ${ENGINE} jordan.tex
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

DROPBOXPATH = Courses

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
