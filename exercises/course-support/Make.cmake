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
	@echo "make cmake_apps"
.PHONY: cmake cmake_apps
cmake :
	@if [ -z "${PROGRAM}" ] ; then \
	    echo "Please specify PROGRAM" && exit 1 \
	 ; fi \
	 && program=${PROGRAM} && program=$${program%%.*} \
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
	 && echo " .. building program : $${program}" \
	 && echo " .. cmake in   : `pwd`" \
	 && echo " .. to prefix  : $$installdir" \
	 && echo " .. using build: $$builddir" \
	 && echo " .. cc         : $${CC}" \
	 && echo " .. cxx        : $${CXX}" \
	 && echo " .. script: $$cmakeliststxt" \
	 && if [ ! -z "${CMAKE_CXX_FLAGS}" ] ; then \
	      export CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS}" \
	       && export CMAKE_LINKER_FLAGS="${CMAKE_CXX_FLAGS}" \
	       && echo " .. extra flags: $${CMAKE_CXX_FLAGS}" \
	    ; fi \
	 \
	 && if [ -z "${CMAKEFILESDIR}" ] ; then \
	      echo "Please set variable CMAKEFILESDIR" && exit 1 ; fi \
	 && cp ${CMAKEFILESDIR}/$$cmakeliststxt ./CMakeLists.txt \
	 && cmake \
	        -B $$builddir \
	        -D PROJECT_NAME=$${program} \
	        $$( if [ "${MODE}" = "mpi" ] ; then \
	              echo "-D MPI_NAME=${LMOD_FAMILY_MPI}" \
	            ; fi ) \
	        -D CMAKE_INSTALL_PREFIX=$$installdir \
	        -D CMAKE_VERBOSE_MAKEFILE=ON \
	 && echo && echo " .. cmaking done" && echo \
	 && ( cd $$builddir && make V=1 && make install ) \
	 && echo && echo " .. installation in $$installdir:" && ls $$installdir && echo
cmake_apps :
	@( \
	  for a in $$( make --no-print-directory listapps | grep -v Warning ) ; do \
	      make --no-print-directory cmake PROGRAM=$$a \
	  ; done \
	 ) 2>&1 | tee cmake.log \
	 && echo "See cmake.log"

localclean ::
	@rm -rf build exe CMake*
