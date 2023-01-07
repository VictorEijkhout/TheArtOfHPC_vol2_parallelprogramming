#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2020
####
#### allreduceinplace.py : reduce in place
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

random_number = random.randint(1,nprocs*nprocs)
print("[%d] random=%d" % (procid,random_number))

#snippet allreduceip
myrandom = np.empty(1,dtype=int)
myrandom[0] = random_number

comm.Allreduce(MPI.IN_PLACE,myrandom,op=MPI.MAX)
#snippet end

if procid==0:
    print("Python numpy:\n  max=%d" % myrandom[0])

