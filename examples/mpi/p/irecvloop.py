#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2022
####
#### irecvloop.c : use non-blocking send/recv to communicate
####     all-to-all with left/right neighbours in a circular fashion
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

#snippet irecvallp
requests = []
sendbuffer = np.empty( nprocs, dtype=int )
recvbuffer = np.empty( nprocs, dtype=int )

for p in range(nprocs):
    left_p = (p-1) % nprocs
    right_p = (p+1) % nprocs
    requests.append( comm.Isend\
        ( sendbuffer[p:p+1],dest=left_p) )
    requests.append( comm.Irecv\
        ( sendbuffer[p:p+1],source=right_p) )
MPI.Request.Waitall(requests)
#snippet end

if procid==0:
    print("All messages received")
    
