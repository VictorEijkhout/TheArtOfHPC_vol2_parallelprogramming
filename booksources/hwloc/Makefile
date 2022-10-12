# -*- makefile -*-
# -*- makefile -*-
################################################################
################################################################
####
#### This make file is part of the source of 
#### `Parallel Programing in Science and Engineering'
#### by Victor Eijkhout, copyright 2013-2021
####
#### Makefile for hwloc figures
####
################################################################
################################################################

include ../makefiles/Make.clean
MAKEFILESDIR := ../${MAKEFILESDIR}
MAKETRACESTRING := "${MAKETRACESTRING} .."

info ::
	@echo "make whatever.pdf where whatever.fig exists"

%.pdf : %.fig
	fig2pdf --pdf --nogv $^
