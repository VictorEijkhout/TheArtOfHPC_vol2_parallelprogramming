#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### scan.py : illustration of comm.Scan
####
################################################################
################################################################

import numpy as np
import random
from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

#snippet scanp
mycontrib = 10+random.randint(1,nprocs)
myfirst = 0
mypartial = comm.scan(mycontrib)
#snippet end
print("[%d] local: %d, partial: %d" % (procid,mycontrib,mypartial))

#snippet scanp
sbuf = np.empty(1,dtype=int)
rbuf = np.empty(1,dtype=int)
sbuf[0] = mycontrib
comm.Scan(sbuf,rbuf)
#snippet end

print("[%d] numpy local: %d, partial: %d" % (procid,mycontrib,rbuf[0]))

