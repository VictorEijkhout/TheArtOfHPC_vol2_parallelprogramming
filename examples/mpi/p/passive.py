#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### passive.py : using passive target synchronization
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

repository = 0 # process that keeps the repository
ninputs = nprocs-1
checksum = nprocs*(nprocs-1)/2.

#snippet fetchopp
if procid==repository:
    # repository process creates a table of inputs
    # and associates it with the window
    win_mem = np.empty( ninputs,dtype=np.float32 )
    win = MPI.Win.Create( win_mem,comm=comm )
else:
    # everyone else has an empty window
    win = MPI.Win.Create( None,comm=comm )
#snippet end

# bookkeeping: which jobs did I process
myjobs = np.zeros( ninputs,dtype=int )

#snippet fetchopp
if procid!=repository:
    contribution = np.empty( 1,dtype=np.float32 )
    contribution[0] = 1.*procid
    table_element = np.empty( 1,dtype=np.float32 )
    win.Lock( repository,lock_type=MPI.LOCK_EXCLUSIVE )
    win.Fetch_and_op( contribution,table_element,repository,0,MPI.SUM)
    win.Unlock( repository )
    #snippet end
    print(procid,"added its contribution to partial sum",table_element[0])

win.Free()

if procid==repository:
    if abs(win_mem[0]-checksum)>1.e-12:
        print("Incorrect result %e s/b %e" % (win_mem[0],checksum))
    print("finished")
