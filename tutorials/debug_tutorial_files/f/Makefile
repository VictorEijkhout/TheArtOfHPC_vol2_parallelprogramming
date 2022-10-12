info ::
	@echo "make <progname>"
	@echo "make clean"

PROGS = hello root2 say square1 root roots square square2
FC = gfortran
% : %.F
	${FC} -g -o $@ $@.F
clean :
	/bin/rm -rf ${PROGS} *~ *.o *.dSYM *.r[0-9]*
