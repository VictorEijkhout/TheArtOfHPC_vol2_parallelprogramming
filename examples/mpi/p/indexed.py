#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### indexed.py : initializing comm world
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
count = 5; totalcount = 15

#snippet indexp
displacements = np.empty(count,dtype=int)
blocklengths = np.empty(count,dtype=int)
source = np.empty(totalcount,dtype=np.float64)
target = np.empty(count,dtype=np.float64)
#snippet end

idcs = [2,3,5,7,11]
for i in range(len(idcs)):
    displacements[i] = idcs[i]
    blocklengths[i] = 1
for i in range(totalcount):
    source[i] = i+.5

#snippet indexp
if procid==sender:
    newindextype = MPI.DOUBLE.Create_indexed(blocklengths,displacements)
    newindextype.Commit()
    comm.Send([source,1,newindextype],dest=the_other)
    newindextype.Free()
elif procid==receiver:
    comm.Recv([target,count,MPI.DOUBLE],source=the_other)
#snippet end

if procid==sender:
    print("finished")
if procid==receiver:
    target_loc = 0
    for block in range(count):
        for element in range(blocklengths[block]):
            source_loc = displacements[block]+element
            if target[target_loc]!=source[source_loc]:
                print("error in src/tar location %d/%d: %e s/b %e" \
                    % (source_loc,target_loc,target[target_loc],source[source_loc]) )
            target_loc += 1
