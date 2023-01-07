#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2022
####
#### buffer4g.py : buffer over the 2G limit
####
#### WRONG we need to make the large buffer through contiguous type
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import time

comm = MPI.COMM_WORLD

ntids = comm.Get_size()
mytid = comm.Get_rank()

dsize = 1000000 * 5000
if mytid==0:
    print(f"Buffer with {dsize} elements")
    data = np.arange(dsize, dtype=np.float64)
    # for i in range(dsize):
    #     data[i] = i+1
    comm.Send([data,MPI.DOUBLE],dest=ntids-1)
elif mytid==ntids-1:
    zdata = np.empty(dsize*2, dtype=np.float64)
    recv_status = MPI.Status()
    comm.Recv([zdata,MPI.DOUBLE],source=0,status=recv_status)
    rsize = recv_status.Get_count(datatype=MPI.DOUBLE)
    print(f"received {rsize} elements")
    


