# -*- makefile -*-
codeclean ::
	-@/bin/rm -rf *.o *~ *.gch a.out *.dSYM MULTI__* events.* \
	    idev[0-9]*.o[0-9]* ddt.o[0-9]* jobtest.o* tautrace_*
