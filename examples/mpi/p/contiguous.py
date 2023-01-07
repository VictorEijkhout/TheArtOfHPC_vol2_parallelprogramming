#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2020
####
#### contiguous.py : use of contiguous data type
####
################################################################
################################################################

import numpy as np
import random # random.randint(1,N), random.random()
from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

sender = 0; receiver = 1; the_other = 1-procid
count = 5

#snippet contiguousp
source = np.empty(count,dtype=np.float64)
target = np.empty(count,dtype=np.float64)
#snippet end

for i in range(count):
    source[i] = i+.5

#snippet contiguousp
if procid==sender:
    newcontiguoustype = MPI.DOUBLE.Create_contiguous(count)
    newcontiguoustype.Commit()
    comm.Send([source,1,newcontiguoustype],dest=the_other)
    newcontiguoustype.Free()
elif procid==receiver:
    comm.Recv([target,count,MPI.DOUBLE],source=the_other)
#snippet end

if procid==sender:
    print("finished")
if procid==receiver:
    for i in range(count):
        if target[i]!=source[i]:
            print("error in location %d: %e s/b %e" % (i,target[i],source[i]))
