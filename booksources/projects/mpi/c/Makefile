info ::
	@echo "Do: make app (where app=${APPS})"

APPS = test helloworld helloworld-seq \
  overlap \
  linear-serial linear-serial-null linear-sendrecv linear-nonblock

ifdef TACC_TAU_DIR
  CC = tau_cc.sh
  CXX = tau_cxx.sh
else
  CC  = mpicc
  CXX = mpicxx
endif

DEBUG=OPT
OPT_flag = -O2
DEBUG_flag = -g
info ::
	@echo "    debug flag: DEBUG=DEBUG/OPT (default: ${DEBUG})]"
%.o : %.cxx
	${CXX} ${${DEBUG}_flag} -D${DEBUG} -c $*.cxx
% : %.o
	${CXX} -o $@ $^

# Example: grid routines
GRID_APPS = grid grid_test grid_blocking grid_nonblocking grid_vectype
APPS += ${GRID_APPS}
grid_dependencies = grid_impl.o grid_tools.o
${GRID_APPS} : ${grid_dependencies}
grid.o grid_blocking.o grid_tools.o : grid.h grid_tools.h

# Example: mandelbrot routines
MANDEL_APPS = mandel_serial mandel_bulk mandel_async mandel_collective
APPS += ${MANDEL_APPS}
mandel_dependencies = mandel_tools.o
${MANDEL_APPS} : ${mandel_dependencies}
${patsubst %,%.o,${MANDEL_APPS}} mandel_tools.o : mandel.h

# rules for submitting to the queue and doing tau analysis
info ::
	@echo
	@echo "make submit EXECUTABLE=<any prog> OPTIONS=<whatever>"
EXECUTABLE = grid_blocking
OPTIONS = pi 4 pj 6 ni 400 nj 600 it 10
submit :
	@export TAU_EXT=`if [ ! -z "${EXECUTABLE}" ] ; then echo "_" ; fi`${EXECUTABLE} ; \
	export TAU_DUMP_DIR=`pwd`/tautrace$$TAU_EXT ; \
	  echo "tau output to: <$$TAU_DUMP_DIR>" ; \
	  mkdir -p $${TAU_DUMP_DIR} ; \
	  TRACEDIR=$${TAU_DUMP_DIR} \
	  PROFILEDIR=$${TAU_DUMP_DIR} \
	    EXECUTABLE=${EXECUTABLE} OPTIONS="${OPTIONS}" \
	      qsub jobscript
ibrun :
	@if [ -z "${EXECUTABLE}" ] ; then \
	   echo "Usage: make ibrun executable option"; exit 1 ; fi
	@export TAU_EXT="_${EXECUTABLE}" ; \
	export TAU_DUMP_DIR=`pwd`/tautrace$$TAU_EXT ; \
	  mkdir -p $${TAU_DUMP_DIR} ; \
	  TAU_TRACE=1   TRACEDIR=$${TAU_DUMP_DIR} \
	  TAU_PROFILE=1 PROFILEDIR=$${TAU_DUMP_DIR} \
	    ibrun ${EXECUTABLE} ${OPTIONS} \
	      qsub jobscript
info ::
	@echo "make tau EXECUTABLE=..."
tau :
	@export HOME=`pwd` ; \
	export TAU_EXT=`if [ ! -z "${EXECUTABLE}" ] ; then echo "_" ; fi`${EXECUTABLE} ; \
	export TAU_DUMP_DIR=`pwd`/tautrace$$TAU_EXT ; \
	  cd $$TAU_DUMP_DIR ; \
	  echo ".. analyzing files in <$$TAU_DUMP_DIR>" ; \
	  rm -f tau.trc tau.edf ; \
	  tau_treemerge.pl ; \
	  tau2slog2 tau.trc tau.edf -o $$HOME/taulog$$TAU_EXT.slog2

info ::
	@echo "make clean       : cleanup but leave executables & slog2"
	@echo "make total_clean : cleanup including executables & slog2"
include ../../Make.inc
clean :: codeclean
total_clean : clean 
	-@/bin/rm -rf ${APPS} *.slog2
