#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2022
####
#### irecvsingle.c : simple request creation
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

#snippet irecvonep
sendbuffer = np.empty( nprocs, dtype=int )
recvbuffer = np.empty( nprocs, dtype=int )

left_p = (procid-1) % nprocs
right_p = (procid+1) % nprocs
send_request = comm.Isend\
    ( sendbuffer[procid:procid+1],dest=left_p)
recv_request = comm.Irecv\
    ( sendbuffer[procid:procid+1],source=right_p)
MPI.Request.Wait(send_request)
MPI.Request.Wait(recv_request)
#snippet end

if procid==0:
    print("All messages received")
    
