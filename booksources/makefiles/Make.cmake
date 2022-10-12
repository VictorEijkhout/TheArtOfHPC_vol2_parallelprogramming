# -*- makefile -*-
################################################################
####
#### This makefile is part of the course
#### Introduction to Scientific Programming in C++ and Fortran
#### by Victor Eijkhout (eijkhout@tacc.utexas.edu)
#### copyright 2017-2022 Victor Eijkhout
####
#### Make.cmake : experimental cmake stuff
####
################################################################

info ::
	@echo && echo "================ Cmake rules:"
	@echo "make cmake"
.PHONY: cmake
cmake :
	@codedir=`pwd` \
	 && echo "Start cmake in: $$codedir" \
	 && installdir=`pwd`/exe && builddir=`pwd`/build \
	 && rm -f CMakeLists.omp.txt \
	     && extension=$$( make --no-print-directory extension ) \
	     && cp ../../../Makefiles/CMakeLists.${MODE}.$${extension}.txt CMakeLists.txt \
	 && rm -rf $$installdir && mkdir -p $$installdir \
	 && rm -rf $$builddir && mkdir -p $$builddir \
	 && echo " .. building program list:" \
	 && if [ ! -z ${SINGLEFILE_APPS} ] ; then \
	        programs="${SINGLEFILE_APPS}" \
	    ; elif [ ! -z "${APPS}" ] ; then \
	        programs="${APPS}" \
	    ; else \
	        echo "Variable APPS not set" && exit 1 \
	    ; fi \
	 && programs=$$( echo $$programs | tr ' ' ';' ) \
	 && echo "    $$programs" && echo \
	 && echo " .. cmake in   : `pwd`" \
	 && echo " .. with code  : $$codedir" \
	 && echo " .. to prefix  : $$installdir" \
	 && echo " .. using build: $$builddir" \
	 && cmake \
	        -S $$codedir -B $$builddir \
	        -D CMAKE_CXX_COMPILER=`which clang++` \
	        -D CMAKE_C_COMPILER=`which clang` \
	        -D CMAKE_Fortran_COMPILER=/opt/local/bin/gfortran \
	        -D CMAKE_INSTALL_PREFIX=$$installdir \
	        -D CMAKE_VERBOSE_MAKEFILE=ON \
	        -D programs=$$programs \
	 && echo && echo " .. cmaking done" && echo \
	 && ( cd $$builddir && make V=1 && make install ) \
	 && echo && echo " .. installation:" && ls $$installdir && echo

clean ::
	@rm -rf build exe CMake*
