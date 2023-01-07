#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### sendblock.py : illustration of the eager limit
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

if procid in [0,nprocs-1]:
    other = nprocs-1-procid
    #snippet sendblockp
    size = 1
    while size<2000000000:
        sendbuf = np.empty(size, dtype=int)
        recvbuf = np.empty(size, dtype=int)
        comm.Send(sendbuf,dest=other)
        comm.Recv(sendbuf,source=other)
        if procid<other:
            print("Send did not block for",size)
        size *= 10
    #snippet end
