#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### vector.py : use of vector data type
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
count = 5; stride = 2

#snippet vectorp
source = np.empty(stride*count,dtype=np.float64)
target = np.empty(count,dtype=np.float64)
#snippet end

for i in range(stride*count):
    source[i] = i+.5

#codesnippet vectorp
if procid==sender:
    newvectortype = MPI.DOUBLE.Create_vector(count,1,stride)
    newvectortype.Commit()
    comm.Send([source,1,newvectortype],dest=the_other)
    newvectortype.Free()
elif procid==receiver:
    comm.Recv([target,count,MPI.DOUBLE],source=the_other)
#codesnippet end

if procid==sender:
    print("finished")
if procid==receiver:
    for i in range(count):
        if target[i]!=source[stride*i]:
            print("error in location %d: %e s/b %e" % (i,target[i],source[stride*i]))
