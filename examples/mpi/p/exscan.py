#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### exscan.py : exclusive scan
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

#snippet exscanp
localsize = 10+random.randint(1,nprocs)
myfirst = 0
mypartial = comm.exscan(localsize,0)
#snippet end

print("[%d] local: %d, partial: %d" % (procid,localsize,mypartial))

