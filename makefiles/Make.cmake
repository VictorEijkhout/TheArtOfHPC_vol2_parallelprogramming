# -*- makefile -*-
################################################################
####
#### This makefile is part of the course
#### Introduction to Scientific Programming in C++ and Fortran
#### by Victor Eijkhout (eijkhout@tacc.utexas.edu)
#### copyright 2017-2023 Victor Eijkhout
####
#### Make.cmake : experimental cmake stuff
####
################################################################

info ::
	@echo && echo "================ Cmake rules:"
	@echo "make cmake PROGRAM=..."
	@echo "make cmake_multi"
.PHONY: cmake cmake_multi
cmake :
	@if [ -z "${PROGRAM}" ] ; then \
	    echo "Please specify PROGRAM" && exit 1 \
	 ; fi \
	 \
	 && installdir=`pwd`/exe && builddir=`pwd`/build \
	 && rm -rf $$installdir && mkdir -p $$installdir \
	 && rm -rf $$builddir && mkdir -p $$builddir \
	 \
	 && if [ -z "${CXX_COMPILER}" ] ; then \
	        echo "Please set CXX_COMPILER" && exit 1 ; fi \
	 && if [ -z "${LMOD_FAMILY_MPI}" ] ; then \
	        echo "Please use Lmod to load MPI" && exit 1 ; fi \
	 && export CXX=${CXX_COMPILER} \
	 && export CC=${C_COMPILER} \
	 && export language=$$( echo ${LANGUAGE} | tr A-Z a-z ) \
	 && export cmakeliststxt=CMakeLists.${MODE}.$${language}.txt \
	 \
	 && echo "Start cmake in: `pwd`" \
	 && echo " .. building program : ${PROGRAM}" \
	 && echo " .. cmake in   : `pwd`" \
	 && echo " .. to prefix  : $$installdir" \
	 && echo " .. using build: $$builddir" \
	 && echo " .. cc         : $${CC}" \
	 && echo " .. cxx        : $${CXX}" \
	 && echo " .. script: $$cmakeliststxt" \
	 \
	 && if [ -z "${CMAKEFILESDIR}" ] ; then \
	      echo "Please set variable CMAKEFILESDIR" && exit 1 ; fi \
	 && cp ${CMAKEFILESDIR}/$$cmakeliststxt ./CMakeLists.txt \
	 && cmake \
	        -B $$builddir \
	        -D PROJECT_NAME=${PROGRAM} \
	        -D MPI_NAME=${LMOD_FAMILY_MPI} \
	        -D CMAKE_INSTALL_PREFIX=$$installdir \
	        -D CMAKE_VERBOSE_MAKEFILE=ON \
	 && echo && echo " .. cmaking done" && echo \
	 && ( cd $$builddir && make V=1 && make install ) \
	 && echo && echo " .. installation in $$installdir:" && ls $$installdir && echo
cmake_multi :
	@cmakelistsdir=`pwd` \
	 && echo "Start cmake in: `pwd`" \
	 && installdir=`pwd`/exe && builddir=`pwd`/build \
	 && rm -f CMakeLists.{mpi,omp}.txt \
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
	 && echo " .. with code  : $$cmakelistsdir" \
	 && echo " .. to prefix  : $$installdir" \
	 && echo " .. using build: $$builddir" \
	 && cmake \
	        -S $$cmakelistsdir -B $$builddir \
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
