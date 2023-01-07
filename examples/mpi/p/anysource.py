#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### anysource.py : initializing comm world
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

if procid==nprocs-1:
    rbuf = np.empty(1,dtype=np.float64)
    for p in range(procid):
        #snippet anysourcep
        rstatus = MPI.Status()
        comm.Recv(rbuf,source=MPI.ANY_SOURCE,status=rstatus)
        print("Message came from %d" % rstatus.Get_source())
        #snippet end
else:        
    sbuf = np.empty(1,dtype=np.float64)
    sbuf = np.empty(1,dtype=np.float64)
    sbuf[0] = 1.
    comm.Send(sbuf,dest=nprocs-1)
    
