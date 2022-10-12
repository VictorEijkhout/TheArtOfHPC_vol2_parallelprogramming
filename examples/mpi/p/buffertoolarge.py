#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### buffertoolarge.py : use of get_count to find actual package size
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import time

comm = MPI.COMM_WORLD

ntids = comm.Get_size()
mytid = comm.Get_rank()
ntests = 100
dsize = 50

if mytid==0:
    data = np.arange(dsize, dtype=np.float64)
    for i in range(dsize):
        data[i] = i+1
    comm.Send([data,MPI.DOUBLE],dest=ntids-1)
elif mytid==ntids-1:
    zdata = np.empty(dsize*2, dtype=np.float64)
    recv_status = MPI.Status()
    comm.Recv([zdata,MPI.DOUBLE],source=0,status=recv_status)
    print("received %d elements" % recv_status.Get_count(datatype=MPI.DOUBLE))
    


